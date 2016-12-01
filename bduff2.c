#include <stdio.h>
#include <stdlib.h>
#include <String.h>

int num = 0;
int trigger = 0;
void select(char* nomeTab, char* atributo, char* operando, char* valor, char* arqAlg, char* arqDad, char* arqCtl){ //gambiarra pra testar o project, ignorar
    if(!trigger){
	trigger++;
	printf("select %s %s %s %s %s %s %s \n",nomeTab,atributo,operando,valor,arqAlg,arqDad,arqCtl);
    char nome_da_tabela_ctl[31];
	char nome_da_tabela_alg[31];
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_ctl,nomeTab);
	strcat(nome_da_tabela_ctl,".ctl");
	strcpy(nome_da_tabela_alg,nomeTab);
	strcat(nome_da_tabela_alg,".alg");
	strcpy(nome_da_tabela_dad,nomeTab);
	strcat(nome_da_tabela_dad,".dad");	
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
	char leitura[51];
	int r = fscanf(fp_tabela_ctl,"%s",leitura);
	while(r==1){

		fprintf(fp_ctl,"%s\n",leitura);
		r = fscanf(fp_tabela_ctl,"%s",leitura);
	}
	fprintf(fp_dad,"FELIPE,NULO,JS,NULO,2434,");
	fclose(fp_ctl);
	fclose(fp_tabela_ctl);
	fclose(fp_dad);
	fclose(fp_alg);
	}
}

void join(char* nomeTabA, char* nomeTabB, char* cond, char* arqAlg, char* arqDad, char* arqCtl){
    printf("join %s %s %s %s %s %s \n",nomeTabA,nomeTabB,cond,arqAlg,arqDad,arqCtl);
	
}

void project(char* nomeTab, int grau, char* atributos, char * arqAlg, char* arqDad, char* arqCtl, int funcAntes){
	printf("project %s %d %s %s %s %s %d \n", nomeTab,grau,atributos,arqAlg,arqDad,arqCtl,funcAntes);
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
		arqDad[strlen(arqDad)-4]='\0'; //apago o final desses arquivos para escrever final.dad/.ctl
		arqCtl[strlen(arqCtl)-4]='\0';
		strcat(arqDad,"Final.dad");
		strcat(arqCtl,"Final.ctl");
	}
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
		while(leituraAux[n]!=','){
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
			while(leituraAux[n]!=',' && leituraAux[n]!='\0'){ //para ler a coluna n da linha
				aux[p] = leituraAux[n];
				n++;				
				p++;
			}
			if (leituraAux[n]=='\0') break;
			for(l=0;l<grau;l++){
				if(cont == indexVetAtributos[l]){//se o atributo da coluna n tiver no meu vetor de atributos salvo em resp
					strcat(resposta,aux);
					strcat(resposta,",");
					break;
				}
			}
			n++;
			cont++;
		}
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
            select(arg1,arg2,arg3,arg4,arg5,nomeDad,nomeCtl);
            
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
            join(arg1,arg2,arg3,arg4,nomeDad,nomeCtl);
            
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
        printf("Fim da leitura do alg %d \n",num);
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

	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "wt");
	if (!fp_tabela_dad) return -1;
        
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "wt");
	if(!fp_tabela_ctl) return -1;
        
        
        
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
        
        //PROGEÇÃO NO SELECT SIMPLES COM WHERE
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


long insert(FILE *fp_sql, long pos){
        fseek(fp_sql, pos, SEEK_SET);
        
        char entrada[31], nome_da_tabela[31];
        
        int r;
	r = fscanf(fp_sql, "%s", entrada); // Lê o INTO e não faz nada
	if (r != 1) return -1;
        r = fscanf(fp_sql, "%s", nome_da_tabela); // Lê nome da tabela
	if (r != 1) return -1;
        r = fscanf(fp_sql, "%s", entrada); // Lê VALUES e não faz nada
	if (r != 1) return -1;
        
	// Cria o nome dos arquivos com as extensões
	char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela);
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela);
	strcat(nome_da_tabela_ctl, ".ctl");
	strcat(nome_da_tabela_dad, ".dad");

	// Abre os arquivos e coloca o cursor no fim do arquivo dad
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "r+t");
	if (!fp_tabela_dad) return -1;
        fseek(fp_tabela_dad, 0L, SEEK_END);
        
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "r+t");
	if(!fp_tabela_ctl) return -1;
        
        // Descobre o valor de N e M
        int n, m, i;
        r = fscanf(fp_tabela_ctl, "%d,%d", &n, &m);
        
        for (i=0; i<n; i++){
            if (i==0){
                char novo[31];
                r = fscanf(fp_sql, "%s", novo);
                if (r != 1) return -1;
                int i=0, j=1;
                while(novo[j] != '\0'){
                    entrada[i]=novo[j];
                    i++;
                    j++;
                }
                entrada[i] = '\0';
            }
            else if(i==(n-1)){
                char novo[31];
                r = fscanf(fp_sql, "%s", novo);
                if (r != 1) return -1;
                int i=0;
                while(novo[i] != ')'){
                    entrada[i]=novo[i];
                    i++;
                }
                entrada[i]='\0';
            }
            else{
                r = fscanf(fp_sql, "%s", entrada);
                if (r != 1) return -1;
            }
            fprintf(fp_tabela_dad, "%s", entrada);
        }
        fprintf(fp_tabela_dad, "\n");
        fclose(fp_tabela_dad);
        
        
        // Atualiza o valor de M no arquivo
        m++;
        rewind(fp_tabela_ctl);
        fprintf(fp_tabela_ctl,"%d,%d", n, m);        
        fclose(fp_tabela_ctl);
        
        pos = ftell(fp_sql);
        return pos;
}


long create_table(FILE *fp_sql, long pos) {
	fseek(fp_sql, pos, SEEK_SET);

	char nome_da_tabela[31];
	char entrada[31];

	int r;
	r = fscanf(fp_sql, "%s", entrada); // Lê o TABLE e não faz nada
	if (r != 1) return -1;
	r = fscanf(fp_sql, "%s", nome_da_tabela); // Lê nome da tabela
	if (r != 1) return -1;
        r = fscanf(fp_sql, "%s", entrada); // Lê o "(" e não faz nada
	if (r != 1) return -1;
        
	// Cria o nome dos arquivos com as extensões
	char nome_da_tabela_ctl[31];
	strcpy(nome_da_tabela_ctl, nome_da_tabela);
	char nome_da_tabela_dad[31];
	strcpy(nome_da_tabela_dad, nome_da_tabela);
	strcat(nome_da_tabela_ctl, ".ctl");
	strcat(nome_da_tabela_dad, ".dad");

	// Abre os arquivos
	// Como o .dad vai ser vazio por enquanto, só cria e fecha ele
	FILE *fp_tabela_dad = fopen(nome_da_tabela_dad, "wt");
	if (!fp_tabela_dad) return 0;
	fclose(fp_tabela_dad);
	FILE *fp_tabela_ctl = fopen(nome_da_tabela_ctl, "wt");
	if(!fp_tabela_ctl) return 0;

	// Cria o N e M como 0 por enquanto
	fprintf(fp_tabela_ctl, "%d,%d\n", 0, 0);

	int n = 0;
	
	//r = fscanf(fp_sql, "%s", entrada); // Lê o parenteses depois de nome_da_tabela e joga fora
	//if (r != 1) return -1;

	while (1) {
		char nome_do_atributo[31], tipo_do_atributo[31], nome_primeiro_atributo[31];
		
		// Lê nome e tipo do atributo
		r = fscanf(fp_sql, "%s", nome_do_atributo);
		if(r != 1) return ftell(fp_sql);
		r = fscanf(fp_sql, "%s", tipo_do_atributo);
		if(r != 1) return ftell(fp_sql);

                
		// Escreve o nome no .ctl fazendo o tratamento com o primeiro atributo que tem "(" junto de si
                if (nome_do_atributo[0] == '(') {
                    int j=0, k=1;
                    while (nome_do_atributo[k] != '\0'){
                        nome_primeiro_atributo[j] = nome_do_atributo[k];
                        j++;
                        k++;
                    }
                    fprintf(fp_tabela_ctl, "%s,", nome_primeiro_atributo);
                }
                else fprintf(fp_tabela_ctl, "%s,", nome_do_atributo);
                
                
		// Escreve o tipo no .ctl
		// Checar se tem virgula, pois se tiver é o fim da linha
		// Se tiver parenteses é o fim do arquivo
		if (strcmp(tipo_do_atributo, "STRING") == 0) {
			fprintf(fp_tabela_ctl, "C");
		} else if(strcmp(tipo_do_atributo, "STRING,") == 0) {
			fprintf(fp_tabela_ctl, "C\n");
			n++;
			continue;
		} else if (strcmp(tipo_do_atributo, "STRING)") == 0) {
			fprintf(fp_tabela_ctl, "C");
			n++;
			break;
		} else if(strcmp (tipo_do_atributo, "INTEGER") == 0) {
			fprintf(fp_tabela_ctl, "I");
		} else if (strcmp (tipo_do_atributo, "INTEGER,") == 0) {
			fprintf(fp_tabela_ctl, "I");
			n++;
			continue;
		} else if (strcmp (tipo_do_atributo, "INTEGER)") == 0) {
			fprintf(fp_tabela_ctl, "I");
			n++;
			break;
		}

		r = fscanf(fp_sql, "%s", entrada);
		if (r != 1) return ftell(fp_sql);

		// Lê os NN, KEY e ORD com as variações de final
		if (strcmp(entrada, "NN,") == 0) {
			fprintf(fp_tabela_ctl, ",nn\n");
			n++;
			continue;
		} else if (strcmp(entrada, "NN)")==0) {
			fprintf(fp_tabela_ctl, ",nn");
			n++;
			break;
		} else if (strcmp(entrada,"NN")==0) {
			fprintf(fp_tabela_ctl, ",nn");
			r = fscanf(fp_sql, "%s", entrada);
			if (r != 1) -1;
				
			if (strcmp(entrada, "KEY,")==0) {
				fprintf(fp_tabela_ctl, ",chv\n");
				n++;
				continue;
			} else if (strcmp(entrada, "KEY")==0) {
				fprintf(fp_tabela_ctl, ",ord,chv\n");
                                fscanf(fp_sql, "%s", entrada);
				n++;
				continue;
			} else if (strcmp(entrada, "KEY)")==0) {
				fprintf(fp_tabela_ctl, ",chv");
				n++;
				break;
			} else if (strcmp(entrada, "ORD,")==0) {
				fprintf(fp_tabela_ctl, ",ord\n");
				n++;
				continue;
			} else if (strcmp(entrada, "ORD)")==0) {
				fprintf(fp_tabela_ctl, ",ord");
				n++;
				break;
			}
		}
	}
	// Guarda retorno
	pos = ftell(fp_sql);
	// Atualiza N
	rewind(fp_tabela_ctl);
	fprintf(fp_tabela_ctl, "%d", n);
	// Fecha arquivo e retorna
	fclose(fp_tabela_ctl);
	return pos;
}



int main(void) {
	char nome_do_arquivo[31];

	printf("BEM VINDO AO BDUFF!\n");
	printf("Insira o nome do arquivo .sql: ");
	scanf("%s", nome_do_arquivo);

	FILE *fp_sql = fopen(nome_do_arquivo, "rt");
	if (!fp_sql) {
		printf("Arquivo especificado nao encontrado!");
		return 0;
	}
       
	char operacao[10];
	int r;
        long pos;

        r = fscanf(fp_sql, "%s", operacao);
        if (r != 1) {
            printf("Nao foi possivel ler arquivo!");
            return 0;
        }
        do{
            if (strcmp(operacao, "CREATE") == 0) {
                pos = create_table(fp_sql, ftell(fp_sql));
            }
            else if (strcmp(operacao, "INSERT") == 0){
                pos = insert(fp_sql, ftell(fp_sql));
            }
            else if (strcmp(operacao, "SELECT") == 0){
                pos = select_sql(fp_sql, ftell(fp_sql));
            }
            fseek(fp_sql, pos, SEEK_SET);
            r = fscanf(fp_sql, "%s", operacao);
        }while(r == 1);
        
        // Fecha o arquivo de entrada
        fclose(fp_sql);                
        printf("Fim do arquivo!\nOperacoes realizadas com sucesso!\n");
}

