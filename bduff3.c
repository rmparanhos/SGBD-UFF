#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct atributo {
	char nome[31];
	char tipo;
	int nn;
	int key;
	int ord;
	struct atributo *prox;
} ATR;


ATR *inicializa_ATR(ATR *atr) {
	atr = (ATR *) malloc(sizeof(ATR));
	atr->nn = 0;
	atr->ord = 0;
	atr->key = 0;
	atr->prox = NULL;
	return atr;
}


int key_check(ATR *atr_inicial, int n) {
	int i;
	for(i = 0; i < n; i++) {
		if(atr_inicial->key) return 1;
		atr_inicial = atr_inicial->prox;
	}
	return 0;
}


int ord_check(ATR *atr_inicial, int n) {
	int i;
	for(i = 0; i < n; i++) {
		if(atr_inicial->ord) return 1;
		atr_inicial = atr_inicial->prox;
	}
	return 0;
}

char charExtra(char *entrada, char *buffer){
	char extra = 0;
	int i;
		for(i = 0; i < strlen(entrada); i++) {
			if(entrada[i] == ','  ||
				entrada[i] == ')' ||
				entrada[i] == '(' ||
				entrada[i] == '\0') {
				extra = entrada[i];
				break;
			}
			buffer[i] = entrada[i];
		}
		buffer[i] = '\0';
		return extra;
}

void create_table(FILE *fp_sql) {

    int i;
	char nome_da_tabela[31], entrada[31], buffer[31];
	int n = 0;

	int r;
	// Lê o TABLE e descarta
	r = fscanf(fp_sql, "%s", entrada);
	if(r != 1) return -1;

	// Lê nome da tabela
	r = fscanf(fp_sql, "%s", entrada);
	if(r != 1) return -1;

	// Trata a entrada
	char char_extra = charExtra(entrada,buffer);

	if(char_extra != '(') {
		// 'Come o ( se ele existir separado do nome'
		r = fscanf(fp_sql, "%s", entrada);
		if(r != 1) return -1;
	}
	strcpy(nome_da_tabela, buffer);

	// Cria encadeamento de atributos
	ATR *atr_inicial, *atr_atual;
	atr_inicial = inicializa_ATR(atr_inicial);
	atr_atual = atr_inicial;

	// Loop dos atributos
	while (1) {
		// Lê nome do atributo
		r = fscanf(fp_sql, "%s", atr_atual->nome);
		if(r != 1) return;

		// Lê o tipo
		r = fscanf(fp_sql, "%s", entrada);
		if(r != 1) return;

		// Trata a entrada
		char char_extra = charExtra(entrada,buffer);

		if(strcmp(buffer, "INTEGER") == 0) {
			atr_atual->tipo = 'I';
		} else if(strcmp(buffer, "STRING") == 0) {
			atr_atual->tipo = 'C';
		}

		// Se for ')' quer dizer que acabou a definição da tabela
		if(char_extra == ')') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			break;
		}
		// Se for ',' passa para o próximo
		if(char_extra == ',') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			continue;
		}

		// Lê o NN
		r = fscanf(fp_sql, "%s", entrada);
		if (r != 1) return;

		// Trata a entrada
		char_extra = charExtra(entrada,buffer);

		if(strcmp(buffer, "NN") == 0) {
			atr_atual->nn = 1;
		} else if(strcmp(buffer, "ORD") == 0) {
			printf("Atributo %s deve ser NN\n", atr_atual->nome);
			return 1;
		} else if(strcmp(buffer, "KEY") == 0) {
			printf("Atributo %s deve ser NN\n", atr_atual->nome);
			return 1;
		}

		// Se for ')' quer dizer que acabou a definição da tabela
		if(char_extra == ')') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			break;
		}
		// Se for ',' passa para o próximo
		if(char_extra == ',') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			continue;
		}

		// Lê o KEY ou ORD
		r = fscanf(fp_sql, "%s", entrada);
		if (r != 1) return;

		// Trata a entrada
		char_extra = charExtra(entrada,buffer);

		if(strcmp(buffer, "ORD") == 0) {
			if(ord_check(atr_inicial, n)) {
				printf("Tabela especifica dois criterios de ordenacao\n");
				return 1;
			}
			atr_atual->ord = 1;
		} else if(strcmp(buffer, "KEY") == 0) {
			if(key_check(atr_inicial, n)) {
				printf("Tabela especifica duas chaves\n");
				return 1;
			}
			atr_atual->key = 1;
		}

		// Se for ')' quer dizer que acabou a definição da tabela
		if(char_extra == ')') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			break;
		}
		// Se for ',' passa para o próximo
		if(char_extra == ',') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			continue;
		}

		// Lê um possível ORD ou KEY respectivamente
		r = fscanf(fp_sql, "%s", entrada);
		if (r != 1) return;

		// Trata a entrada
		char_extra = charExtra(entrada,buffer);

		if(strcmp(buffer, "ORD") == 0) {
			if(ord_check(atr_inicial, n)) {
				printf("Tabela especifica dois criterios de ordenacao\n");
				return 1;
			}
			atr_atual->ord = 1;
		} else if(strcmp(buffer, "KEY") == 0) {
			if(key_check(atr_inicial, n)) {
				printf("Tabela especifica duas chaves\n");
				return 1;
			}
			atr_atual->key = 1;
		}

		// Se for ')' quer dizer que acabou a definição da tabela
		if(char_extra == ')') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			break;
		}
		// Se for ',' passa para o próximo
		if(char_extra == ',') {
			ATR *atr_prox;
			atr_prox = inicializa_ATR(atr_prox);
			atr_atual->prox = atr_prox;
			atr_atual = atr_prox;
			n++;
			continue;
		}
	}


	// Cria o nome dos arquivos com as extensões
	char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela);
	strcat(nome_da_tabela_ctl, ".ctl");
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela);
	strcat(nome_da_tabela_dad, ".dad");

	// Abre os arquivos
	// Como o .dad vai ser vazio por enquanto, só cria e fecha ele
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "wt");
	if (!fp_tabela_dad) return 0;
	fclose(fp_tabela_dad);
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "wt");
	if(!fp_tabela_ctl) return 0;

	// Escreve o N,M
	fprintf(fp_tabela_ctl, "%d,%d\n", n, 0);

	atr_atual = atr_inicial;
	for(i = 0; i < n; i++) {
		fprintf(fp_tabela_ctl, "%s,%c", atr_atual->nome, atr_atual->tipo);
		if(atr_atual->nn) {
			fprintf(fp_tabela_ctl, ",nn");
		}
		if(atr_atual->ord) {
			fprintf(fp_tabela_ctl, ",ord");
		}
		if(atr_atual->key) {
			fprintf(fp_tabela_ctl, ",chv");
		}
		fprintf(fp_tabela_ctl, "\n");

		atr_atual = atr_atual->prox;
	}
	
	// Fecha arquivo e retorna
	fclose(fp_tabela_ctl);

}



int key_unica_check(char *nome_da_tabela, char *entrada, int n_atributo, int m) {
	// Gera o nome do .dad
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela);
	strcat(nome_da_tabela_dad, ".dad");

	// Abre o arquivo
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "r");
    char linha[255], buffer[255];

	int i, atr_count, ib;
	for(i = 0; i < m; i++) {
        ib=0;
		fscanf(fp_tabela_dad, "%s", linha);
        atr_count  = 0;
		int j;
		for(j = 0; j < strlen(linha); j++) {
			if(linha[j] == ',') {
				atr_count++;
				buffer[ib] = '\0';

				if(atr_count - 1 == n_atributo) {
					if(strcmp(buffer, entrada) == 0) {
						return 1;
					} else {
						continue;
					}
				}else ib=0;

			} else {
				buffer[ib] = linha[j];
				ib++;
			}
		}

	}
	return 0;
}

ATR **structify_ctl(char *nome_da_tabela, int *n, int *m) {

	// Cria o nome do arquivo ctl
	char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela);
	strcat(nome_da_tabela_ctl, ".ctl");

	// Abre o arquivo
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "r");
	if(!fp_tabela_ctl) {
		printf("Tabela especificada inexistente\n");
		return NULL;
	}

	// Lê N e M
	fscanf(fp_tabela_ctl, "%d,%d", n, m);

	// Prepara lista de ATR;
	ATR **atr_lista = (ATR **) malloc(sizeof(ATR *) * *n);

	// Lê o primeiro atributo
	char entrada[255];
	int i;
	for(i = 0; i < *n; i++) {
		atr_lista[i] = (ATR *) malloc(sizeof(ATR));
		atr_lista[i] = inicializa_ATR(atr_lista[i]);

		fscanf(fp_tabela_ctl, "%s", entrada);

		// Bota no buffer o nome
		char buffer[255];
		int j, i_b = 0;
		for(j = 0; j < strlen(entrada); j++) {
			if(entrada[j] == ',') {
				buffer[i_b] == '\0';
				i_b = j;
				j = strlen(entrada);
			} else {
				buffer[i_b] = entrada[j];
				i_b++;
			}
		}
		j = i_b;

		// Copia o buffer pra struct
		strcpy(atr_lista[i]->nome, buffer);

		// Coloca na struct o tipo
		atr_lista[i]->tipo = entrada[j + 1];

		// Checa nn
		j+= 2;

		if(entrada[j] == '\0') {
			continue;
		} else {
			atr_lista[i]->nn = 1;
		}

		// Checa ord e chv
		j+= 3;
		if(entrada[j] == '\0') {
			continue;
		} else if(entrada[j + 1] == 'o') {
			atr_lista[i]->ord = 1;
			if(entrada[j+5] == 'c') {
                atr_lista[i]->key = 1;
			}
		} else if(entrada[j + 1] == 'c') {
			atr_lista[i]->key = 1;
		}

	}

	return atr_lista;
}


void dad_to_list(char *nome_tabela_dad, int n, int m, char tuplas[m][n][31]) {
	// Abre o .dad pra leitura
	FILE *fp_tabela_dad = fopen(nome_tabela_dad, "r");

	char buffer[255], linha[255];
	int atr_lidos = 0;

	// Para cada linha do .dad
	int i, j;
	for(i = 0; i < m; i++) {
		// Lê a linha
		fscanf(fp_tabela_dad, "%s", linha);

		// Percorre a linha salvando cada atributo
		// no buffer e logo depois na matriz
		int ib = 0;
		for(j = 0; j <= strlen(linha); j++) {
			if(linha[j] == ',' || linha[j] == '\0') {
				buffer[ib] = '\0';
				atr_lidos++;
				strcpy(tuplas[i][atr_lidos - 1], buffer);
				ib = 0;
			} else {
				buffer[ib] = linha[j];
				ib++;
			}
		}
		atr_lidos = 0;
	}
	fclose(fp_tabela_dad);

	return;
}


void ord_dad(char *nome_da_tabela, int atributo_ord, int n, int m) {
	// Gera nome do .dad
	char nome_tabela_dad[31];
	strcpy(nome_tabela_dad, nome_da_tabela);
	strcat(nome_tabela_dad, ".dad");

	// Cria uma matriz de strings onde as linhas são as
	// linhas do .dad e as colunas os atributos de cada linha
	// e manda como argumento para dad_to_list preencher
	char tuplas[m][n][31];
	dad_to_list(nome_tabela_dad, n, m, tuplas);

	// Transforma ctl em struct e descobre o tipo de atributo_ord
	ATR **atributos = structify_ctl(nome_da_tabela, &n, &m);
	char tipo = atributos[atributo_ord]->tipo;

	// Algoritimo de quicksort
	char swap[255];
	int troca = 1;
	while(troca) {
		troca = 0;
		int i,j;
		for(i = 0; i < m - 1; i++) {
			if(tipo == 'I') {
				int i1, i2;
				i1 = atoi(tuplas[i][atributo_ord]);
				i2 = atoi(tuplas[i + 1][atributo_ord]);
				if(i1 > i2) {
					for(j = 0; j < n; j++){
						strcpy(swap, tuplas[i + 1][j]);
						strcpy(tuplas[i + 1][j], tuplas[i][j]);
						strcpy(tuplas[i][j], swap);
					}			
					troca = 1;
				}
			} else if(tipo == 'C') {
				if(strcmp(tuplas[i][atributo_ord], tuplas[i + 1][atributo_ord]) > 0) {
					for(j = 0; j < n; j++){
						strcpy(swap, tuplas[i + 1][j]);
						strcpy(tuplas[i + 1][j], tuplas[i][j]);
						strcpy(tuplas[i][j], swap);
					}			
					troca = 1;
				}
			}
		}
	}

	// Escreve o arquivo ordenado no .dad
	FILE *fp_tabela_dad = fopen(nome_tabela_dad, "w");
	int i, j;
	for(i = 0; i < m; i++) {
		for(j = 0; j < n; j++) {
			fprintf(fp_tabela_dad, "%s", tuplas[i][j]);
			if(j < n - 1) {
				fprintf(fp_tabela_dad, ",");
			}
		}
		fprintf(fp_tabela_dad, "\n");
	}
	fclose(fp_tabela_dad);
}


void insert(FILE *fp_sql) {


	char entrada[255], descarte[31], nome_da_tabela[31];

	// Lê INTO e ignora
	fscanf(fp_sql, "%s", descarte);

	// Lê nome da tabela
	fscanf(fp_sql, "%s", nome_da_tabela);


	// Cria array de atributos e recebe N e M
	int n, m;
	ATR **atributos = structify_ctl(nome_da_tabela, &n, &m);

	if(!atributos) {
		printf("Nao foi possivel ler atributos\n");
		exit(1);
	}
	// Lê VALUES
	fscanf(fp_sql, "%s", descarte);
	// Lê valores
	fscanf(fp_sql, "%s", entrada);

	// Lidar com esses valores
	char buffer[255], saida_temp[1055];
	saida_temp[0] = '\0';
	int atributo_ord;
	int i =0, ib;
	int j = 1;
	for(i = 0; i < n; i++) {

   		ib = 0;
		for(;j < strlen(entrada); j++) {
			if(entrada[j] == ',' ||
				entrada[j] == ')') {
				buffer[ib] = '\0';
				ib = j + 1;
				j = strlen(entrada);
			} else {
				buffer[ib] = entrada[j];
				ib++;
			}
		}
		j = ib;
		// Checa se é o critério de ordenação
		if(atributos[i]->ord) atributo_ord = i;

		// Checar chave única
		if(atributos[i]->key) {
			char atributo[255];
			strcpy(atributo, buffer);
			if(atributo[0] == '"') {
				atributo[0] = '\'';
				atributo[strlen(atributo) - 1] = '\'';
			}

			if(key_unica_check(nome_da_tabela, atributo, i, m)){
				printf("Chave duplicada no atributo %d\n", i + 1);
				return 1;
			}
		}

		// Checa se é NULO
		if(strcmp(buffer, "NULO") == 0) {
			if(!atributos[i]->nn) {
				strcat(saida_temp, buffer);
				if(i < n - 1) strcat(saida_temp, ",");
			} else {
				printf("Atributo %d nao pode ser nulo\n", i + 1);
				return 1;
			}
		// Checa se é um numero baseado na tabela ASCII
		} else if(buffer[0] >= 48 && buffer[0] <= 57) {
			if(atributos[i]->tipo == 'I') {
				strcat(saida_temp, buffer);
				if(i < n - 1) strcat(saida_temp, ",");
			} else {
				printf("Atributo %d eh do tipo STRING\n", i + 1);
				return 1;
			}
		// Checa se é string
		} else if(buffer[0] == '"') {
			if(atributos[i]->tipo == 'C') {
				buffer[0] = '\'';
				buffer[strlen(buffer) - 1] = '\'';
				strcat(saida_temp, buffer);
				if(i < n - 1) strcat(saida_temp, ",");
			} else {
				printf("Atributo %d eh do tipo INTEGER\n", i + 1);
				return 1;
			}

		} else {
			printf("Atributo %d invalido\n", i + 1);
			return 1;
		}
	}
	m++;

	// Gera o nome do .dad e do .ctl
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela);
	strcat(nome_da_tabela_dad, ".dad");
	char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela);
	strcat(nome_da_tabela_ctl, ".ctl");

	// Abre o arquivo
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "a");
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "r+");
	fseek(fp_tabela_ctl, 0, SEEK_SET);

	fprintf(fp_tabela_ctl, "%d,%d\n", n, m);
	fprintf(fp_tabela_dad, "%s\n", saida_temp);

	fclose(fp_tabela_dad);
	fclose(fp_tabela_ctl);

	// Ordena o .dad final
	ord_dad(nome_da_tabela, atributo_ord, n, m);
}


int num = 0;
void BD_select(char* nomeTab, char* atributo, char* operando, char* valor, char* arqAlg, char* arqDad, char* arqCtl){ 
    
	char nome_da_tabela_ctl[31];
	char nome_da_tabela_alg[31];
	char nome_da_tabela_dad[31];
	
	strcpy(nome_da_tabela_ctl,nomeTab);
	strcat(nome_da_tabela_ctl,".ctl");
	strcpy(nome_da_tabela_alg,arqAlg);
	strcat(nome_da_tabela_alg,".alg");
	strcpy(nome_da_tabela_dad,nomeTab);
	strcat(nome_da_tabela_dad,".dad");	
	
	
    strcat(arqAlg,".alg");
	FILE *fp_saida_alg = fopen(arqAlg, "rt");
	if (!fp_saida_alg) {
		printf("Problemas com arquivo.alg");
		exit(1);
	}
	
	FILE *fp_saida_dad = fopen(arqDad, "wt");
	if (!fp_saida_dad) {
		printf("Problemas com arquivo.dad");
		exit(1);
	}
	FILE *fp_saida_ctl = fopen(arqCtl, "wt");
	if (!fp_saida_ctl) {
		printf("Problemas com arquivo.ctl");
		exit(1);
	}
	FILE *fp_entrada_ctl = fopen(nome_da_tabela_ctl, "rt");
	if (!fp_entrada_ctl) {
		printf("Problemas com tabela.ctl");
		exit(1);
	}
	
	FILE *fp_entrada_dad = fopen(nome_da_tabela_dad, "rt");
	if (!fp_entrada_dad) {
		printf("Problemas com tabela.dad");
		exit(1);
	}
	//arquivos abertos e com os nomes certos a partir daqui
	int grau;
	int cardinalidade;
	int r = fscanf(fp_entrada_ctl, "%d,%d",&grau,&cardinalidade);
	char leitura[51];
	char nomeAtributo[31];
	char tipoAtributo;
	int indexAtributo = 0;
	
	r = fscanf(fp_entrada_ctl,"%s",&leitura);
	while(r != -1){ //descubro o indice do atributo e o tipo do atributo
		int i = 0;
		while(leitura[i] != ','){
			nomeAtributo[i] = leitura[i]; //escrevo nome do atributo em nomeAtributo
			i++;
		}
		if(strcmp(nomeAtributo,atributo)==0){ //se nomeAtributo = atributo(atributo q será usado para comparacao)
			i++;
			while(leitura[i] != ','){
				tipoAtributo = leitura[i]; //escrevo tipo do atributo em tipoAtrbuto
				i++;
			}
			break;
		}
		r = fscanf(fp_entrada_ctl,"%s",&leitura);
		indexAtributo++;
	}
	rewind(fp_entrada_ctl);
	
	cardinalidade = 0;
	char atrSeraComparado[31];
	r = fscanf(fp_entrada_dad,"%s",&leitura); //começo a leitura do dad
	while(r != -1){ //escrevo os resultados da seleçao em saida.dad
		int i,j=0;
		for(i=0;i<=indexAtributo;i++){ //uso indice do atributo para descobrir qual atributo eu quero
			memset(atrSeraComparado,0,31);
			int k = 0;
			while(leitura[j]!=',' && leitura[j]!='\0'){
				atrSeraComparado[k] = leitura[j]; //escrevo ele em atrSeraComparado
				j++;
				k++;
			}
			j++;
		}
		
		if(tipoAtributo == 'I'){ //se inteiro
			int iAtr;
			int iVal;
			sscanf(atrSeraComparado,"%d",&iAtr); //escrevo o atributo q quero comparar numa variavel inteira
			sscanf(valor,"%d",&iVal); //escrevo o valor q veio pelo argumento numa variavel inteira
			//testo as condiçoes, e se for satisfeita, escrevo na saida e aumento a cardinalidade
			if(strcmp(operando,"=")==0){
				if(iAtr == iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,"<")==0){
				if(iAtr < iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,"<=")==0){
				if(iAtr <= iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,">")==0){
				if(iAtr > iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,">=")==0){
				if(iAtr >= iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,"<>")==0){
				if(iAtr != iVal){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
		}
		else{ //se string, mesma coisa
			if(strcmp(operando,"=")==0){
				if(strcmp(atrSeraComparado,valor)==0){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
			if(strcmp(operando,"<>")==0){
				if(!strcmp(atrSeraComparado,valor)!=0){
					fprintf(fp_saida_dad,"%s\n",leitura);
					cardinalidade++;
				}
			}
		}
		r = fscanf(fp_entrada_dad,"%s",&leitura);
		memset(atrSeraComparado,0,31);
	}
	int aux1,aux2;
	fprintf(fp_saida_ctl,"%d,%d\n",grau,cardinalidade); //escrevo grau e cardinalidade (q eu obtive na escrita em dad) em ctl
	fscanf(fp_entrada_ctl,"%d,%d",&aux1,&aux2); //pulo a primeira linha 
	r = fscanf(fp_entrada_ctl,"%s",&leitura);
	while(r!=-1){ //copio entrada ctl para saida ctl
		fprintf(fp_saida_ctl,"%s\n",leitura);
		r = fscanf(fp_entrada_ctl,"%s",&leitura);
	}
	//fecho arquivos
	fclose(fp_saida_dad);
	fclose(fp_entrada_dad);
	fclose(fp_saida_ctl);
	fclose(fp_entrada_ctl);
	fclose(fp_saida_alg);
}

void join(char* nomeTabA, char* nomeTabB, char* cond, char* arqAlg, char* arqDad, char* arqCtl, int teveSelect){
    //printf("join %s %s %s %s %s %s %d \n",nomeTabA,nomeTabB,cond,arqAlg,arqDad,arqCtl,teveSelect);
    char nome_da_tabela_ctl_A[31];
	char nome_da_tabela_dad_A[31];
	char nome_da_tabela_ctl_B[31];
	char nome_da_tabela_dad_B[31];
	char nome_da_tabela_alg[31];
	
	if(!teveSelect){//caso eu nao tenha tido uma seleçao antes, os arquivos q serao lidos sao o originais
		strcpy(nome_da_tabela_ctl_A,nomeTabA);
		strcat(nome_da_tabela_ctl_A,".ctl");
		strcpy(nome_da_tabela_dad_A,nomeTabA);
		strcat(nome_da_tabela_dad_A,".dad");	
		strcpy(nome_da_tabela_ctl_B,nomeTabB);
		strcat(nome_da_tabela_ctl_B,".ctl");
		strcpy(nome_da_tabela_dad_B,nomeTabB);
		strcat(nome_da_tabela_dad_B,".dad");	
		strcpy(nome_da_tabela_alg,arqAlg);
		strcat(nome_da_tabela_alg,".alg");
	}
	else{//se nao, os arquivos serão os editados pelas funçoes anteriores
		strcpy(nome_da_tabela_ctl_A,arqCtl);
		strcpy(nome_da_tabela_dad_A,arqDad);
		strcpy(nome_da_tabela_ctl_B,nomeTabB);
		strcat(nome_da_tabela_ctl_B,".ctl");
		strcpy(nome_da_tabela_dad_B,nomeTabB);
		strcat(nome_da_tabela_dad_B,".dad");
		strcpy(nome_da_tabela_alg,arqAlg);
		strcat(nome_da_tabela_alg,".alg");
		arqDad[strlen(arqDad)-4]='\0'; //apago o final desses arquivos para escrever intermediario.dad/.ctl
		arqCtl[strlen(arqCtl)-4]='\0';
		strcat(arqDad,"Intermediario.dad");
		strcat(arqCtl,"Intermediario.ctl");
	}
	strcat(arqAlg,".alg");
	FILE *fp_saida_alg = fopen(arqAlg, "rt");
	if (!fp_saida_alg) {
		printf("Problemas com arquivo.alg");
		exit(1);
	}
	
	FILE *fp_saida_dad = fopen(arqDad, "wt");
	if (!fp_saida_dad) {
		printf("Problemas com saida.dad");
		exit(1);
	}
	FILE *fp_saida_ctl = fopen(arqCtl, "wt");
	if (!fp_saida_ctl) {
		printf("Problemas com saida.ctl");
		exit(1);
	}
	FILE *fp_entrada_ctl_A = fopen(nome_da_tabela_ctl_A, "rt");
	if (!fp_entrada_ctl_A) {
		printf("Problemas com entradaA.ctl");
		exit(1);
	}
	FILE *fp_entrada_dad_A = fopen(nome_da_tabela_dad_A, "rt");
	if (!fp_entrada_dad_A) {
		printf("Problemas com entradaA.dad");
		exit(1);
	}
	FILE *fp_entrada_ctl_B = fopen(nome_da_tabela_ctl_B, "rt");
	if (!fp_entrada_ctl_B){
		printf("Problemas com entradaB.ctl");
		exit(1);
	}
	
	FILE *fp_entrada_dad_B = fopen(nome_da_tabela_dad_B, "rt");
	if (!fp_entrada_dad_B) {
		printf("Problemas com entradaB.dad");
		exit(1);
	}
	char leitura[41];
	char leituraAux[31];
	int cardinalidade = 0;
	int grauA = 0;
	int cardinalidadeA = 0;
	int grauB = 0;
	int cardinalidadeB = 0;
	int indiceA = 0;
	int indiceB = 0;
	char atrA[31] = "";
	char atrB[31] = "";
	int i=0;
	while(cond[i]!='='){ //escrevo primeiro atributo do join em atrA
		atrA[i] = cond[i];
		i++;
	}
	i++;
	int j = 0;
	while(cond[i]!='\0'){ //escrevo segundo atributo do join em atrB
		atrB[j] = cond[i];
		i++;
		j++;
	}
	i=0;
	fscanf(fp_entrada_ctl_A,"%d,%d",&grauA,&cardinalidadeA); //leio grau e cardinalidade da primeira tabela pra pular a primeira linha
	int r = fscanf(fp_entrada_ctl_A,"%s",&leitura); //leitura do ctl da primeira tabela
	while(1){
		i=0;
		
		while(leitura[i]!=','){ 
			leituraAux[i] = leitura[i]; //escrevo nome do atributo da linha em leituraAux
			i++;
		}
		if(strcmp(leituraAux,atrA)==0){ //se for o atributo certo, saio do loop
			break;
		}
		indiceA++; //incremento essa variavel pra saber qual é o indice do atributo
		memset(leitura,0,41); //zero o vetor leitura
		memset(leituraAux,0,31); //zero o vetor leituraAux
		r = fscanf(fp_entrada_ctl_A,"%s",&leitura); //leio proxima linha
	}
	memset(leitura,0,41);
	memset(leituraAux,0,31);
	fscanf(fp_entrada_ctl_B,"%d,%d",&grauB,&cardinalidadeB); //segunda tabela agora
	r = fscanf(fp_entrada_ctl_B,"%s",&leitura);
	while(1){
		i = 0;
	    while(leitura[i]!=','){
			leituraAux[i] = leitura[i];
			i++;
		}
		if(strcmp(leituraAux,atrB)==0){
			break;
		}
		indiceB++;
		memset(leitura,0,41);
		memset(leituraAux,0,31);
		r = fscanf(fp_entrada_ctl_A,"%s",&leitura);
	}
	
	char leituraA[41]="";
	char leituraB[41]="";
	r = fscanf(fp_entrada_dad_A,"%s",&leituraA); //leitura dos dados da primeira tabela
	while(r==1){
		char compA[31]="";
		char compB[31]="";
		int k=0;
		for(i=0;i<=indiceA;i++){ //uso indiceA para encontrar qual atributo eu quero da primeira tabela
			memset(compA,0,31);
			j=0;
			while(leituraA[k]!=',' && leituraA[k]!='\0'){
				compA[j]=leituraA[k];  //escrevo tal atributo em compA
				j++;
				k++;
			}
			k++;
		}
	
		int s = fscanf(fp_entrada_dad_B,"%s",&leituraB); //leitura dos dados da segunda tabela
		while(s==1){
			k=0;
			for(i=0;i<=indiceB;i++){ //uso indiceB para encontrar qual atributo eu quero da segunda tabela
				memset(compB,0,31);
				j=0;
				while(leituraB[k]!=','&&leituraB[k]!='\0'){
					compB[j]=leituraB[k]; //escrevo tal atributo em compB
					j++;
					k++;
				}
				k++;
			}
			if(strcmp(compA,compB)==0){ //se for igual
				strcat(leituraA,",");
				strcat(leituraA,leituraB); //concateno as duas leituras
				fprintf(fp_saida_dad,"%s\n",leituraA); //escrevo a linha do dad, na saida
				cardinalidade++; //incremento cardinlidade
			}
			memset(leituraB,0,31);
			memset(compB,0,31);
			s = fscanf(fp_entrada_dad_B,"%s",leituraB); //continuo leitura da tabela 2
		}
		memset(leituraA,0,31);
		memset(compA,0,31);
		rewind(fp_entrada_dad_B); //retorno ponteiro pro inicio
		r = fscanf(fp_entrada_dad_A,"%s",leituraA); //continuo leitura da tabela 1
	}
	
	rewind(fp_entrada_ctl_A);
	rewind(fp_entrada_ctl_B);
	r = fscanf(fp_entrada_ctl_A,"%d,%d",&grauA,&cardinalidadeA); //leio primeira linha do ctl da primeira tabela
	r = fscanf(fp_entrada_ctl_B,"%d,%d",&grauB,&cardinalidadeB);  //leio primeira linha do ctl da segunda tabela
	grauA += grauB; // o grau do resultado é grauA+grauB
	fprintf(fp_saida_ctl,"%d,%d\n",grauA,cardinalidade); //escrevo na saida ctl, grau e cardinalidade
	r = fscanf(fp_entrada_ctl_A,"%s",&leitura);
	while(r==1){ //escrevo linhas do ctl da primeira tabela em saida ctl
		char leituraAuxA[41]="";
		strcat(leituraAuxA,"A"); //escrevo o novo nome do atributo
		if(strcmp(nomeTabA,nomeTabB)==0) strcat(leituraAuxA,"1"); 
		strcat(leituraAuxA,leitura);
		fprintf(fp_saida_ctl,"%s\n",leituraAuxA); //escrevo linha no arquivo
		r = fscanf(fp_entrada_ctl_A,"%s",&leitura);
	}
	int iAux1,iAux2;
	fscanf(fp_entrada_ctl_B,"%d,%d",&iAux1,&iAux2);
	r = fscanf(fp_entrada_ctl_B,"%s",&leitura);
	while(r==1){//mesma coisa q o anterior
		char leituraAuxB[41]="";
		if(strcmp(nomeTabA,nomeTabB)==0) strcat(leituraAuxB,"A2");
		else strcat(leituraAuxB,"B");
		strcat(leituraAuxB,leitura);
		fprintf(fp_saida_ctl,"%s\n",leituraAuxB);
		r = fscanf(fp_entrada_ctl_B,"%sn",&leitura);
	}
	//fecho os arquivos
	fclose(fp_saida_alg);
	fclose(fp_saida_dad);
	fclose(fp_saida_ctl);
	fclose(fp_entrada_ctl_A);
	fclose(fp_entrada_dad_A);
	fclose(fp_entrada_ctl_B);
	fclose(fp_entrada_dad_B);
	
}

void project(char* nomeTab, int grau, char* atributos, char * arqAlg, char* arqDad, char* arqCtl, int funcAntes){
	
	printf("\nSelecao %d:\n",num);
	char nome_da_tabela_ctl[31];
	char nome_da_tabela_alg[31];
	char nome_da_tabela_dad[31];
	
	if(!funcAntes){//caso eu tenha apenas uma projeçao, os arquivos q serao lidos sao o originais
		strcpy(nome_da_tabela_ctl,nomeTab);
		strcat(nome_da_tabela_ctl,".ctl");
		strcpy(nome_da_tabela_alg,arqAlg);
		strcat(nome_da_tabela_alg,".alg");
		strcpy(nome_da_tabela_dad,nomeTab);
		strcat(nome_da_tabela_dad,".dad");	
		
	}
	else{//se nao, os arquivos serão os editados pelas funçoes anteriores
		strcpy(nome_da_tabela_ctl,arqCtl);
		strcpy(nome_da_tabela_alg,arqAlg);
		strcat(nome_da_tabela_alg,".alg");
		strcpy(nome_da_tabela_dad,arqDad);
		//arqDad[strlen(arqDad)-4]='\0'; //apago o final desses arquivos para escrever final.dad/.ctl
		//arqCtl[strlen(arqCtl)-4]='\0';
		
		
	}
	arqDad[strlen(arqDad)-4]='\0'; //apago o final desses arquivos para escrever final.dad/.ctl
	arqCtl[strlen(arqCtl)-4]='\0';
	strcat(arqDad,"Final.dad");
	strcat(arqCtl,"Final.ctl");
	//abertura dos arquivos
	strcat(arqAlg,".alg");

	FILE *fp_alg = fopen(arqAlg, "rt");
	if (!fp_alg) {
		printf("Problemas com arquivo.alg");
		exit(1);
	}
	FILE *fp_dad = fopen(arqDad, "wt");
	if (!fp_dad) {
		printf("Problemas com arquivo.dad");
		exit(1);
	}
	FILE *fp_ctl = fopen(arqCtl, "wt");
	if (!fp_ctl) {
		printf("Problemas com arquivo.ctl");
		exit(1);
	}
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "rt");
	if (!fp_tabela_ctl) {
		printf("Problemas com tabela.ctl");
		exit(1);
	}
	
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "rt");
	if (!fp_tabela_dad) {
		printf("Problemas com tabela.dad");
		exit(1);
	}
	
	//escrevo grau e cardinalidade em saida.ctl
	int grauOriginal = 0;
	int cardinalidade = 0;
	fscanf(fp_tabela_ctl,"%d,%d", &grauOriginal, &cardinalidade);
	fprintf(fp_ctl,"%d,%d \n",grau,cardinalidade);
	
	//crio um array de strings, com os atributos
	char vetAtributos[grau][31];
	int indexVetAtributos[grau];
	int i,j=0,k=0,n=0;
	for(i=0;i<grau;i++){
		while(1){
			if(atributos[j]==',' || atributos[j] == '\0'){
				j++;
				vetAtributos[i][k] ='\0';
				k=0;
				break;	
			} 
			vetAtributos[i][k] = atributos[j];
			k++;
			j++;
		}		
	}
	
	char* leitura[31];
	int posAtributo = 0;
	int cont = 0;
	while(1){
		n=0;
		char aux[31] = "0";
		int r = fscanf(fp_tabela_ctl,"%s",leitura); //leio a linha do ctl
		if(r!=1) break;
		char leituraAux[31];
		strcpy(leituraAux,leitura); //copio a linha para um array para poder correr por ele
		while(leituraAux[n]!=','&&leituraAux[n]!='\0'){
			aux[n] = leituraAux[n]; //salvo o nome do atributo
			n++;
		}
		n=0;
		while(n<grau){
			if(strcmp(aux,vetAtributos[n])==0){ //verifico se o nome do atributo esta no meu array de atributos
				fprintf(fp_ctl,"%s \n",leitura); //escrevo a linha em saida.ctl
				indexVetAtributos[cont] = posAtributo; //salvo a posiçao do atributo num vetor 
				cont++;
				break;
			}
			n++;
		}
		posAtributo ++;		
	}
	
	
	while(1){
		n=0;
		int l;
		int cont=0;
		int r = fscanf(fp_tabela_dad,"%s",leitura); //leio a linha do dad
		if(r!=1) break;
		char aux[31] = "";
		char leituraAux[31];
		char resposta[91]="";
		strcpy(leituraAux,leitura); //novamente, para poder correr pela string
		while(1){
			char aux[31] = "";
			int p = 0;
			while(leituraAux[n]!=','&&leituraAux[n]!='\0'){ //para ler a coluna n da linha
				aux[p] = leituraAux[n];
				n++;				
				p++;
			}
			for(l=0;l<grau;l++){
				if(cont == indexVetAtributos[l]){//se o atributo da coluna n tiver no meu vetor de atributos salvo em resp
					strcat(resposta,aux);
					strcat(resposta,",");
					break;
				}
			}
			n++;
			cont++;
			if (leituraAux[n]=='\0') break;
		}
		resposta[strlen(resposta)-1]='\0';//apagar virgula do final
		fprintf(fp_dad,"%s\n",resposta); //escrevo resposta no arquivo
		printf("%s\n",resposta); //print na tela
	}
	fclose(fp_dad);
	fclose(fp_tabela_dad);
	fclose(fp_ctl);
	fclose(fp_tabela_ctl);
	fclose(fp_alg);
	
}

void le_alg(char* nomeArq, char* nomeDad, char* nomeCtl){
	FILE *fp_alg = fopen(nomeArq, "rt");
	if (!fp_alg) {
		printf("Problemas com arquivo.alg");
		exit(1);
	}
	//variaveis auxiliares pros argumentos
	char operacao[91]="0"; 
	char arg1[15]="0"; 
	char arg2[15]="0";
	char arg3[15]="0";
	char arg4[15]="0";
	char arg5[15]="0";
	int  arg6=0;
	int r;
    long pos;
    int funcAntesProject = 0;
    int teveSelect = 0;
    
    r = fscanf(fp_alg, "%s", operacao);//leio tudo pra operaçao
    if (r != 1) {
        printf("Nao foi possivel ler arquivo.alg!");
        exit(1);
    }
    do{ //divido a string operaçao conforme a funçao utilizada
		if (operacao[0] == 'S') {
            int i = 2;
        	int k = 0;
        	int j;
            while(operacao[i]!=','){
            	arg1[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=','){
            	arg2[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=','){
            	arg3[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=','){
            	arg4[k] = operacao[i];
            	k++;
            	i++;
			}
			i++;
			k=0;
			while(operacao[i]!=')'){
            	arg5[k] = operacao[i];
            	k++;
            	i++;
			}
			funcAntesProject = 1;
			teveSelect = 1;            		
            BD_select(arg1,arg2,arg3,arg4,arg5,nomeDad,nomeCtl);
            
        }
        else if (operacao[0] == 'P'){
        	int i = 2;
        	int k = 0;
        	int j;
            while(operacao[i]!=','){
            	arg1[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			arg6 = (int) operacao[i] - '0';
			
			i++;
			i++;
			for(j=0;j<arg6;j++){ //arg2 = atributo1,atributo2,atributo3
				while(1){
            		arg2[k] = operacao[i];
            		k++;
            		i++;
            		if(operacao[i]==',') break;
				}
			}
			i++;
			k=0;
			while(operacao[i]!=')'){
            	arg3[k] = operacao[i];
            	k++;
            	i++;
			}
                		
            project(arg1,arg6,arg2,arg3,nomeDad,nomeCtl,funcAntesProject);
            
        }
        else if (operacao[0] == 'J'){
            int i = 2;
        	int k = 0;
        	int j;
            while(operacao[i]!=','){
            	arg1[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=','){
            	arg2[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=','){
            	arg3[k] = operacao[i];
            	k++;
            	i++;
			}
			
			i++;
			k=0;
			while(operacao[i]!=')'){
            	arg4[k] = operacao[i];
            	k++;
            	i++;
			}			         		
			funcAntesProject = 1;
            join(arg1,arg2,arg3,arg4,nomeDad,nomeCtl,teveSelect);
            
        }
        //fseek(fp_sql, pos, SEEK_SET);
        r = fscanf(fp_alg, "%s", operacao);
        //reset strings
	    memset(arg1, 0, 15);
	    memset(arg2, 0, 15);
	    memset(arg3, 0, 15);
	    memset(arg4, 0, 15);
	    memset(arg5, 0, 15);
	    arg6 = 0;
        }while(r == 1);
        
        // Fecha o arquivo de entrada
        fclose(fp_alg);                
        //printf("Fim da leitura do alg %d \n",num);
}

long select_sql(FILE *fp_sql, long pos){
       
        fseek(fp_sql, pos, SEEK_SET);
        
        char entrada[31], nome_da_tabela[31], atributos[61];
        
        int r, n_atributos=0, i=0, temp=0, a=0;
        
        // Lê os atributos a serem selecionados
        r = fscanf(fp_sql, "%s", atributos); 
        if (r != 1) return -1;         
                    
        // Lê o FROM e não faz nada
        r = fscanf(fp_sql, "%s", entrada); 
        if (r != 1) return -1; 
        
        //lê o nome da tabela
        r = fscanf(fp_sql, "%s", nome_da_tabela); 
            if (r != 1) return -1;
              
        
        //Cria arquivo .alg
        char nome_da_tabela_alg[31], aux[5], nome_da_tabela_simples[31];
        strcpy(nome_da_tabela_alg, "ALGEBRA_");
        strcpy(nome_da_tabela_simples, "ALGEBRA_");
        num++;
	    sprintf(aux, "%d", num);
        strcat(nome_da_tabela_alg, aux);
        strcat(nome_da_tabela_alg, ".alg");
        strcat(nome_da_tabela_simples, aux);        
     
        //ABRE OS PONTEIROS PARA OS ARQUIVOS CTL E DAD QUE SERÃO GERADOS AO FIM DESSA ROTINA.
        
    char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela_simples);
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela_simples);
	strcat(nome_da_tabela_ctl, ".ctl");
	strcat(nome_da_tabela_dad, ".dad");

	//FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "wt");
	//if (!fp_tabela_dad) return -1;
        
	//FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "wt");
	//if(!fp_tabela_ctl) return -1;
        
        
        
        //contar a quantidade de atributos
        int qtd=1;
        a=0;
        while(atributos[a] != '\0'){
            if(atributos[a] == ',') qtd++;
            a++;
        }
        
        
        FILE *fp_tabela_alg = fopen(nome_da_tabela_alg, "wt");
        if (!fp_tabela_alg) return -1;
        
        while(nome_da_tabela[temp] != '\0') temp++;
        temp--;
        
        // PROJEÇÃO NO SELECT SIMPLES
        if(nome_da_tabela[temp] == ';'){
                
                int j=0;  
                while(nome_da_tabela[j] != ';'){
                    nome_da_tabela[j] = nome_da_tabela[j];
                    j++;
                }
                nome_da_tabela[j] = '\0';
                

                fprintf(fp_tabela_alg, "P(%s,%d,%s,%s)\n", nome_da_tabela, qtd, atributos, nome_da_tabela_simples);
                fclose(fp_tabela_alg);
                
                // ATUALIZAR OS VALORES DAS RELAÇÕES GERADAS
                
               
                
                // MANDAR FAZER OPERAÇÃO DE ALGEBRA RELACIONAL COM OS ARQUIVOS GERADOS
                le_alg(nome_da_tabela_alg,nome_da_tabela_dad, nome_da_tabela_ctl);
                pos = ftell(fp_sql);
                return pos;    
        } 
        
        //PROJEÇÃO NO SELECT SIMPLES COM WHERE
        // Lê o que vem em seguida
        r = fscanf(fp_sql, "%s", entrada); 
        if (r != 1) return -1;

        //Se é um SELECT simples com condição
        if(strcmp(entrada, "WHERE") == 0){
                // Lê a condição
                char atr[31], ope[5], valor[31];
                r = fscanf(fp_sql, "%s", entrada); 
                if (r != 1) return -1;
                
                int j=0, k=0;         
                while(entrada[j]!= '\0'){
                    if ((entrada[j] == '<')||(entrada[j] == '>')||(entrada[j]=='=')){
                        ope[0] = entrada[j];
                        break;
                    }
                    atr[j] = entrada[j];
                    j++;
                }
                atr[j] = '\0';
                j++;
                if ((entrada[j] == '=') || (entrada[j] == '>')){
                        ope[1] = entrada[j];
                        ope[2] = '\0';
                        j++;
                }
                else ope[1] = '\0';
                while(entrada[j]!= ';'){
                    valor[k] = entrada[j];
                    k++;
                    j++;
                }
                valor[k] = '\0';
                
                
                // Já estou com os nomes certos nas variáveis certas!
                fprintf(fp_tabela_alg, "S(%s,%s,%s,%s,%s)\n", nome_da_tabela, atr, ope, valor, nome_da_tabela_simples);
                fprintf(fp_tabela_alg, "P(%s,%d,%s,%s)\n", nome_da_tabela, qtd, atributos, nome_da_tabela_simples);
                fclose(fp_tabela_alg);
                atributos[0]='\0';
                
                // ATUALIZAR OS VALORES DAS RELAÇÕES GERADAS
                
                
                // MANDAR FAZER OPERAÇÃO DE ALGEBRA RELACIONAL COM OS ARQUIVOS GERADOS
                
                le_alg(nome_da_tabela_alg,nome_da_tabela_dad,nome_da_tabela_ctl);
                pos = ftell(fp_sql);
                return pos;                  
        }
        
        //SELECT COM JOIN
        if(strcmp(entrada, "JOIN") == 0){
            
            //Descobre o nome da segunda tabela
            char tabela_2[31], condicao_juncao[31];
            
            r = fscanf(fp_sql, "%s", tabela_2);
            if(r!=1) return -1;
            
            r = fscanf(fp_sql, "%s", entrada); // Lê o ON
            if(r!=1) return -1;
            
            r = fscanf(fp_sql, "%s", condicao_juncao); // Lê a codição de junção
            if(r!=1) return -1;            
           
            temp=0;
            while(condicao_juncao[temp] != '\0') temp++;
            temp--;
            
            //JOIN COM WHERE
            if(condicao_juncao[temp] != ';'){
                // Lê o WHERE
                r = fscanf(fp_sql, "%s", entrada); 
                if (r != 1) return -1;
            
                // Lê a condição
                char atr[31], ope[5], valor[31];
                r = fscanf(fp_sql, "%s", entrada); 
                if (r != 1) return -1;
              
                int j=0, k=0;         
                while(entrada[j]!= '\0'){
                    if ((entrada[j] == '<')||(entrada[j] == '>')||(entrada[j]=='=')){
                        ope[0] = entrada[j];
                        break;
                    }
                    atr[j] = entrada[j];
                    j++;
                }
                atr[j] = '\0';
                j++;
                if ((entrada[j] == '=') || (entrada[j] == '>')){
                    ope[1] = entrada[j];
                    ope[2] = '\0';
                    j++;
                }
                else ope[1] = '\0';
                while(entrada[j]!= ';'){
                    valor[k] = entrada[j];
                    k++;
                    j++;
                }
                valor[k] = '\0';            
                
                fprintf(fp_tabela_alg, "S(%s,%s,%s,%s,%s)\n", nome_da_tabela, atr, ope, valor, nome_da_tabela_simples);
                
            }
            int j=0;  
            while(condicao_juncao[j] != ';'){
                    condicao_juncao[j] = condicao_juncao[j];
                    j++;
            }
            condicao_juncao[j] = '\0';
           fprintf(fp_tabela_alg, "J(%s,%s,%s,%s) \n", nome_da_tabela, tabela_2, condicao_juncao, nome_da_tabela_simples);
            fprintf(fp_tabela_alg, "P(%s,%d,%s,%s)\n", nome_da_tabela, qtd, atributos, nome_da_tabela_simples);
            fclose(fp_tabela_alg);
            
            // ATUALIZAR OS VALORES DAS RELAÇÕES GERADAS
            
           
            // MANDAR FAZER OPERAÇÃO DE ALGEBRA RELACIONAL COM OS ARQUIVOS GERADOS
           
            le_alg(nome_da_tabela_alg,nome_da_tabela_dad,nome_da_tabela_ctl);
            pos = ftell(fp_sql);
            return pos;        
        }
}

int main(void) {
	char nome_do_arquivo[31];

	printf("BEM VINDO AO BDUFF!\n");
	printf("Insira o nome do arquivo .sql: ");
	scanf("%s", nome_do_arquivo);

	FILE *fp_sql = fopen(nome_do_arquivo, "rt");
	if (!fp_sql) {
		printf("Arquivo especificado nao encontrado!\n");
		return 0;
	}

	char operacao[10];
	int r;

	// Loop para ler os comandos SQL

	r = fscanf(fp_sql, "%s", operacao);
	if (r != 1) {
		printf("Nao foi possivel ler arquivo!");
		return 0;
	}
	while(r == 1) {
		if (strcmp(operacao, "CREATE") == 0) {
			create_table(fp_sql);
		}else if (strcmp(operacao, "INSERT") == 0) {
			insert(fp_sql);
		}else if (strcmp(operacao, "SELECT") == 0){
			select_sql(fp_sql,ftell(fp_sql));
		}
		r = fscanf(fp_sql, "%s", operacao);
	}
    fclose(fp_sql);
	return 0;
}


