#include <stdio.h>
#include <stdlib.h>

/* ============================================================
   POLYNOMIAL CALCULATOR USING LINKED LIST
   Each term of a polynomial is stored as a node:
   coefficient, exponent, pointer to next term.
   Terms are always kept sorted in descending order of exponent,
   and like terms (same exponent) are automatically combined.
   ============================================================ */

typedef struct Node {
    int coeff;
    int exp;
    struct Node *next;
} Node;

/* ---------- Utility: create a new node ---------- */
Node* createNode(int coeff, int exp) {
    Node *newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->coeff = coeff;
    newNode->exp = exp;
    newNode->next = NULL;
    return newNode;
}

/* ---------- Insert a term into polynomial (sorted, combines like terms) ---------- */
Node* insertTerm(Node *head, int coeff, int exp) {
    if (coeff == 0) return head; // skip zero coefficient terms

    Node *newNode;

    // Empty list
    if (head == NULL) {
        return createNode(coeff, exp);
    }

    // Insert at head if exponent is greater than head's exponent
    if (exp > head->exp) {
        newNode = createNode(coeff, exp);
        newNode->next = head;
        return newNode;
    }

    // If same exponent as head, combine
    if (exp == head->exp) {
        head->coeff += coeff;
        if (head->coeff == 0) {
            Node *temp = head;
            head = head->next;
            free(temp);
        }
        return head;
    }

    // Traverse to find correct position
    Node *curr = head;
    while (curr->next != NULL && curr->next->exp > exp) {
        curr = curr->next;
    }

    // Combine with existing term of same exponent
    if (curr->next != NULL && curr->next->exp == exp) {
        curr->next->coeff += coeff;
        if (curr->next->coeff == 0) {
            Node *temp = curr->next;
            curr->next = temp->next;
            free(temp);
        }
    } else {
        newNode = createNode(coeff, exp);
        newNode->next = curr->next;
        curr->next = newNode;
    }

    return head;
}

/* ---------- Input a polynomial from user ---------- */
Node* inputPolynomial() {
    Node *head = NULL;
    int n, coeff, exp, i;

    printf("Enter number of terms: ");
    scanf("%d", &n);

    for (i = 0; i < n; i++) {
        printf("Term %d -> Enter coefficient and exponent (e.g. 3 2 for 3x^2): ", i + 1);
        if (scanf("%d %d", &coeff, &exp) != 2) {
            printf("  Invalid input. Stopping term entry early.\n");
            break;
        }
        if (exp < 0) {
            printf("  Exponent cannot be negative. Term skipped.\n");
            continue;
        }
        head = insertTerm(head, coeff, exp);
    }
    return head;
}

/* ---------- Display polynomial ---------- */
void displayPolynomial(Node *head, const char *name) {
    printf("%s(x) = ", name);
    if (head == NULL) {
        printf("0\n");
        return;
    }

    Node *curr = head;
    int first = 1;
    while (curr != NULL) {
        if (!first) {
            printf(curr->coeff >= 0 ? " + " : " - ");
        } else if (curr->coeff < 0) {
            printf("-");
        }
        int absCoeff = abs(curr->coeff);

        if (curr->exp == 0) {
            printf("%d", absCoeff);
        } else if (curr->exp == 1) {
            if (absCoeff == 1) printf("x");
            else printf("%dx", absCoeff);
        } else {
            if (absCoeff == 1) printf("x^%d", curr->exp);
            else printf("%dx^%d", absCoeff, curr->exp);
        }

        first = 0;
        curr = curr->next;
    }
    printf("\n");
}

/* ---------- Add two polynomials ---------- */
Node* addPolynomials(Node *p1, Node *p2) {
    Node *result = NULL;
    Node *t1 = p1, *t2 = p2;

    while (t1 != NULL) {
        result = insertTerm(result, t1->coeff, t1->exp);
        t1 = t1->next;
    }
    while (t2 != NULL) {
        result = insertTerm(result, t2->coeff, t2->exp);
        t2 = t2->next;
    }
    return result;
}

/* ---------- Subtract p2 from p1 ---------- */
Node* subtractPolynomials(Node *p1, Node *p2) {
    Node *result = NULL;
    Node *t1 = p1, *t2 = p2;

    while (t1 != NULL) {
        result = insertTerm(result, t1->coeff, t1->exp);
        t1 = t1->next;
    }
    while (t2 != NULL) {
        result = insertTerm(result, -(t2->coeff), t2->exp);
        t2 = t2->next;
    }
    return result;
}

/* ---------- Multiply two polynomials ---------- */
Node* multiplyPolynomials(Node *p1, Node *p2) {
    Node *result = NULL;
    Node *t1 = p1;

    if (p1 == NULL || p2 == NULL) return NULL;

    while (t1 != NULL) {
        Node *t2 = p2;
        while (t2 != NULL) {
            result = insertTerm(result, t1->coeff * t2->coeff, t1->exp + t2->exp);
            t2 = t2->next;
        }
        t1 = t1->next;
    }
    return result;
}

/* ---------- Evaluate polynomial at given x ---------- */
double evaluatePolynomial(Node *head, double x) {
    double result = 0.0;
    Node *curr = head;
    while (curr != NULL) {
        double term = curr->coeff;
        int i;
        for (i = 0; i < curr->exp; i++) {
            term *= x;
        }
        result += term;
        curr = curr->next;
    }
    return result;
}

/* ---------- Derivative of polynomial ---------- */
Node* derivativePolynomial(Node *head) {
    Node *result = NULL;
    Node *curr = head;
    while (curr != NULL) {
        if (curr->exp != 0) {
            result = insertTerm(result, curr->coeff * curr->exp, curr->exp - 1);
        }
        curr = curr->next;
    }
    return result;
}

/* ---------- Free polynomial memory ---------- */
Node* freePolynomial(Node *head) {
    Node *curr = head;
    while (curr != NULL) {
        Node *temp = curr;
        curr = curr->next;
        free(temp);
    }
    return NULL;
}

/* ---------- Menu ---------- */
void printMenu() {
    printf("\n=====================================================\n");
    printf("         POLYNOMIAL CALCULATOR (LINKED LIST)\n");
    printf("=====================================================\n");
    printf(" 1.  Create/Re-enter Polynomial A\n");
    printf(" 2.  Create/Re-enter Polynomial B\n");
    printf(" 3.  Display Polynomial A\n");
    printf(" 4.  Display Polynomial B\n");
    printf(" 5.  Add A + B\n");
    printf(" 6.  Subtract A - B\n");
    printf(" 7.  Multiply A * B\n");
    printf(" 8.  Evaluate Polynomial A at x\n");
    printf(" 9.  Evaluate Polynomial B at x\n");
    printf("10.  Derivative of Polynomial A\n");
    printf("11.  Derivative of Polynomial B\n");
    printf("12.  Exit\n");
    printf("=====================================================\n");
    printf("Enter your choice: ");
}

int main() {
    Node *polyA = NULL, *polyB = NULL, *result = NULL;
    int choice;
    double x, value;

    printf("Welcome to the Polynomial Calculator!\n");
    printf("Enter each term as: coefficient exponent\n");
    printf("Example: for 3x^2 + 2x - 5 -> enter (3 2), (2 1), (-5 0)\n");

    do {
        printMenu();
        if (scanf("%d", &choice) != 1) {
            printf("\nInput ended. Exiting.\n");
            break;
        }

        switch (choice) {
            case 1:
                polyA = freePolynomial(polyA);
                printf("\n--- Enter Polynomial A ---\n");
                polyA = inputPolynomial();
                break;

            case 2:
                polyB = freePolynomial(polyB);
                printf("\n--- Enter Polynomial B ---\n");
                polyB = inputPolynomial();
                break;

            case 3:
                displayPolynomial(polyA, "A");
                break;

            case 4:
                displayPolynomial(polyB, "B");
                break;

            case 5:
                result = freePolynomial(result);
                result = addPolynomials(polyA, polyB);
                printf("\nResult of Addition:\n");
                displayPolynomial(result, "A+B");
                break;

            case 6:
                result = freePolynomial(result);
                result = subtractPolynomials(polyA, polyB);
                printf("\nResult of Subtraction:\n");
                displayPolynomial(result, "A-B");
                break;

            case 7:
                result = freePolynomial(result);
                result = multiplyPolynomials(polyA, polyB);
                printf("\nResult of Multiplication:\n");
                displayPolynomial(result, "A*B");
                break;

            case 8:
                printf("Enter value of x: ");
                scanf("%lf", &x);
                value = evaluatePolynomial(polyA, x);
                printf("A(%.2f) = %.4f\n", x, value);
                break;

            case 9:
                printf("Enter value of x: ");
                scanf("%lf", &x);
                value = evaluatePolynomial(polyB, x);
                printf("B(%.2f) = %.4f\n", x, value);
                break;

            case 10: {
                Node *deriv = derivativePolynomial(polyA);
                printf("\nDerivative of A:\n");
                displayPolynomial(deriv, "A'");
                freePolynomial(deriv);
                break;
            }

            case 11: {
                Node *deriv = derivativePolynomial(polyB);
                printf("\nDerivative of B:\n");
                displayPolynomial(deriv, "B'");
                freePolynomial(deriv);
                break;
            }

            case 12:
                printf("Exiting... Freeing memory. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 12);

    // Final cleanup
    polyA = freePolynomial(polyA);
    polyB = freePolynomial(polyB);
    result = freePolynomial(result);

    return 0;
}