/**
 * Nom:     Oriol
 * Cognoms: Benítez Bravo
 * NIU:     1566931
 *
 * Nom:     Oliver Einard
 * Cognoms: Tarragó Boardman
 * NIU:     1527541
 */


#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    unsigned n1, n2, estat;
} aresta;

typedef struct {
    unsigned *nodes;
    unsigned *arvis;
    int naresvisit;
} estructuracami;

unsigned llegir_arestes(FILE *fitxer, aresta larestes[], unsigned num_arestes);
unsigned max_arestes(unsigned num_nodes, aresta larestes[], unsigned num_arestes);
unsigned calcular_valencies(aresta *larestes, unsigned num_arestes, unsigned *valencies, unsigned num_nodes);
unsigned arestesdelnode(aresta larestes[], unsigned num_arestes, unsigned nn, unsigned int llistap[]);


unsigned maximals_node(aresta larestes[], unsigned num_arestes, unsigned node);

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Ús: Pr5Ex512 fitxer.txt\nfitxer.txt ha de contenir:\n"
               "A la primera línia el número de nodes\n"
               "A la segona línia el número d'arestes\n"
               "A les següents línies, les arestes en format: node1 node2\n");
        return -1;
    }

    unsigned num_nodes, num_arestes;
    FILE *fitxer = fopen(argv[1], "r");
    fscanf(fitxer, "%u", &num_nodes);
    fscanf(fitxer, "%u", &num_arestes);

    aresta larestes[num_arestes];
    estructuracami cami;
    cami.nodes = (unsigned *) malloc((num_arestes + 1) * sizeof(unsigned));
    cami.arvis = (unsigned *) malloc(num_arestes * sizeof(unsigned));

    if (cami.nodes == NULL || cami.arvis == NULL) {
        printf("No s'ha pogut reservar memòria per a l'estructura de camí.");
        return -1;
    }

    if (llegir_arestes(fitxer, larestes, num_arestes) != num_arestes) {
        printf("El número d'arestes llegides del fitxer no correspón al número d'arestes de la segona línia del fitxer.");
        return -1;
    }

    unsigned max_arestesXnode = max_arestes(num_nodes, larestes, num_arestes);
    unsigned arestespos[max_arestesXnode];

    cami.naresvisit = 0;
    for (int i = 0; i < num_arestes; i++) {
        printf("Aresta %d -> [%d,%d]\n", i, larestes[i].n1, larestes[i].n2);
    }

    unsigned valencies[num_nodes];
    unsigned node_max_valencia = calcular_valencies(larestes, num_arestes, valencies, num_nodes);
    printf("\nLes valencies dels nodes són:\n");
    for (unsigned i = 0; i < 4; i++) {
        printf("El node %u té: %u arestes\n", i, valencies[i]);
    }
    printf("\nEl node amb més arestes és el %u\n", node_max_valencia);

    printf("\nIntroduïu el node del qual volgueu saber els seus camins maximals: ");
    unsigned node;
    scanf("%u", &node);
    printf("\n");
    maximals_node(larestes, num_arestes, node);

    printf("\nNode Inici: ");
    scanf("%u", cami.nodes);
    unsigned npos = arestesdelnode(larestes, num_arestes, cami.nodes[0], arestespos);
    bool seguir = true;
    do {
        if (npos > 0) {
            printf("Teniu %d arestes per triar: \n", npos);
            for (int i = 0; i < npos; i++) {
                printf("%d -> [%d,%d]\n", arestespos[i], larestes[arestespos[i]].n1, larestes[arestespos[i]].n2);
            }

            bool arestaposible = false;
            do {
                printf("Trieu: ");
                scanf("%u", &cami.arvis[cami.naresvisit]);
                for (int i = 0; i < npos; i++) {
                    if (arestespos[i] == cami.arvis[cami.naresvisit]) {
                        arestaposible = true;
                    }
                }
            } while (!arestaposible);

            larestes[cami.arvis[(cami.naresvisit)]].estat = 1;
            (cami.naresvisit)++;
            if (larestes[cami.arvis[(cami.naresvisit) - 1]].n1 == cami.nodes[(cami.naresvisit) - 1]) {
                cami.nodes[cami.naresvisit] = larestes[cami.arvis[(cami.naresvisit) - 1]].n2;
            } else {
                cami.nodes[cami.naresvisit] = larestes[cami.arvis[(cami.naresvisit) - 1]].n1;
            }
            for (int i = 0; i < cami.naresvisit; i++) {
                printf("%d [%d,%d] ", cami.nodes[i],
                       larestes[cami.arvis[i]].n1, larestes[cami.arvis[i]].n2);
            }
            printf("%d\n", cami.nodes[cami.naresvisit]);
            npos = arestesdelnode(larestes, num_arestes, cami.nodes[cami.naresvisit], arestespos);
        } else {
            printf("No queden arestes disponibles.\n");
            printf("Arestes visitades (%d):\n", cami.naresvisit);
            for (int i = 0; i < cami.naresvisit; i++) {
                printf("%d ", cami.arvis[i]);
            }
            seguir = false;
        }
    } while (seguir);
    printf("\n");
    return 0;
}

unsigned llegir_arestes(FILE *fitxer, aresta larestes[], unsigned num_arestes) {
    unsigned i = 0, n1, n2;
    while ((fscanf(fitxer, "%u %u", &n1, &n2)) != EOF) {
        if (i < num_arestes) {
            larestes[i].n1 = n1;
            larestes[i].n2 = n2;
            larestes[i].estat = 0;
        }
        ++i;
    }
    return i;
}

unsigned max_arestes(unsigned num_nodes, aresta larestes[], unsigned num_arestes) {
    unsigned arestesXnode[num_nodes], max = 0;
    for (int i = 0; i < num_nodes; ++i) {
        arestesXnode[i] = 0;
    }

    for (int i = 0; i < num_arestes; ++i) {
        ++arestesXnode[larestes[i].n1];
        ++arestesXnode[larestes[i].n2];
    }

    for (int i = 0; i < num_nodes; ++i) {
        if (arestesXnode[i] > max) {
            max = arestesXnode[i];
        }
    }

    return max;
}

unsigned arestesdelnode(aresta larestes[], unsigned num_arestes, unsigned nn, unsigned int llistap[]) {
    unsigned i, nombre = 0;
    for (i = 0; i < num_arestes; i++) {
        if (larestes[i].estat == 0 && (larestes[i].n1 == nn || larestes[i].n2 == nn)) {
            llistap[nombre] = i;
            nombre++;
        }
    }
    return nombre;
}


unsigned int calcular_valencies(aresta *larestes, unsigned num_arestes, unsigned *valencies, unsigned num_nodes) {
    for (unsigned i = 0; i < num_nodes; i++) {
        valencies[i] = 0;
    }

    for (unsigned i = 0; i < num_arestes; i++) {
        valencies[larestes[i].n1]++;
        valencies[larestes[i].n2]++;
    }

    unsigned node_max_valencia = 0;
    for (unsigned i = 0; i < num_nodes; i++) {
        if (valencies[i] > valencies[node_max_valencia]) {
            node_max_valencia = i;
        }
    }

    return node_max_valencia;
}

unsigned _maximals_node(aresta larestes[], unsigned num_arestes, unsigned desde_node, unsigned node_actual,
                        unsigned cami[], unsigned cami_index) {

    bool maximal = true;
    unsigned maximals = 0;
    for (int i = 0; i < num_arestes; ++i) {
        aresta aresta_ = larestes[i];
        if (aresta_.estat == 0) {
            unsigned proper_node = -1;
            if (aresta_.n1 == node_actual) {
                proper_node = aresta_.n2;
            } else if (aresta_.n2 == node_actual) {
                proper_node = aresta_.n1;
            }

            if (proper_node != -1) {
                maximal = false;
                larestes[i].estat = 1;
                cami[cami_index] = i;
                maximals += _maximals_node(larestes, num_arestes, desde_node, proper_node, cami, cami_index + 1);
                larestes[i].estat = 0;
            }
        }
    }

    if (maximal && cami_index > 0) {
        maximals += 1;
        printf("Arestes visitades (%u):", cami_index);
        for (int i = 0; i < cami_index; ++i) {
            printf(" %u", cami[i]);
        }
        printf("\n");
    }

    return maximals;
}

unsigned maximals_node(aresta larestes[], unsigned num_arestes, unsigned node) {
    printf("Node inici: %u\n", node);
    aresta larestes_[num_arestes];
    for (int i = 0; i < num_arestes; ++i) {
        larestes_[i] = larestes[i];
        larestes_[i].estat = 0;
    }
    unsigned cami[num_arestes];
    unsigned maximals = _maximals_node(larestes_, num_arestes, node, node, cami, 0);
    printf("S'han trobat %u camins\n", maximals);
    return maximals;
}
