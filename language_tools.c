#include "manage_data.c"

void LoadDataFile(String30 filename, String150 sentence){
	FILE *fp_importfile;
	char ch[2];
	
	strcat(filename, ".txt");
	if((fp_importfile = fopen(filename, "r")) == NULL){
		printf("File %s does not exist.\n\n", filename);
	}
	else{
		strcpy(sentence, "");
		while((fscanf(fp_importfile, "%c", ch)) == 1){
			strcat(sentence, ch);
		}
	}
	fclose(fp_importfile);
}

void Tokenize(String150 sentence, String150 Words[], int *pWordCount){
	int i;
	int j = 0;
	
	*pWordCount = 0;
	strcat(sentence, " ");
	for(i = 0; i < strlen(sentence); i++){
		if(sentence[i] != ' ' && sentence[i] != '\0' && sentence[i] != ',' && sentence[i] != ';' &&
			sentence[i] != '?' && sentence[i] != '!' && sentence[i] != '.'){
			Words[*pWordCount][j] = sentence[i];
			j++;
		}
		else{
			Words[*pWordCount][j] = '\0';
			j = 0;
			(*pWordCount)++;
		}
	}
}

int GetLanguages(Entry entrylist[], String20 languages[]){
	int i, j;
	int k = 0;
	int entrycount = GetEntryCount(entrylist);
	int languagecount;
	
	languagecount = 0;
	//copy all languages to array
	for(i = 0; i < entrycount; i++){
		for(j = 0; j < entrylist[i].paircount; j++){
			strcpy(languages[k], entrylist[i].LanguageSet[j].language);
			k++;
			languagecount++;
		}
	}
	
	// remove duplicates from array
	for(i = 0; i < languagecount; i++){
		for(j = i + 1; j < languagecount; j++){
			if((strcmp(languages[i], languages[j])) == 0){
				for(k = j; k < languagecount - 1; k++){
					strcpy(languages[k], languages[k + 1]);
				}
				languagecount--;
				j--;
			}
		}
	}

	return languagecount;
}

void IdentifyMainLanguage(Entry entrylist[], String150 sentence){
	int i, j, k, m;
	int entrylist_count;
	int languagecount;
	String150 Words[MAX_ENTRY];
	int nWords;
	String20 Languages[MAX_ENTRY];
	int Tally[MAX_ENTRY];
	int max_index;
	
	entrylist_count = GetEntryCount(entrylist);
	languagecount = GetLanguages(entrylist, Languages);
	Tokenize(sentence, Words, &nWords);
	
	for(i = 0; i < languagecount; i++){
		Tally[i] = 0;
	}
	
	for(i = 0; i < nWords; i++){
		for(j = 0; j < entrylist_count; j++){
			for(k = 0; k < entrylist[j].paircount; k++){
				if(strcmp(Words[i], entrylist[j].LanguageSet[k].translation) == 0){
					for(m = 0; m < languagecount; m++){
						if(strcmp(Languages[m], entrylist[j].LanguageSet[k].language) == 0){
							Tally[m]++;
						}
					}
					k = entrylist[j].paircount;
					j = entrylist_count;
				}
			}
		}
	}
	
	max_index = 0;
	for(i = 1; i < languagecount; i++){
		if(Tally[max_index] < Tally[i])
			max_index = i;
	}
	printf("Sentence:\n%s\n", sentence);
	if(Tally[max_index] != 0){
		printf("The main language is %s.\n", Languages[max_index]);
		Tally[max_index] = 0;
		max_index = 0;
		for(i = 1; i < languagecount; i++){
			if(Tally[max_index] < Tally[i])
				max_index = i;
		}
		if(Tally[max_index] != 0){
			printf("The second main language is %s.\n", Languages[max_index]);
		}
	}
	else
		printf("The main language is %s\n", "unknown");	
}

void SimpleTranslation(Entry entrylist[], String20 source_language, String20 output_language, String150 sentence){
	int i, j, k, m;
	int entrylist_count;
	int nWords;
	String150 Words[150];
	int translation_found;
	String150 translated_text;
	
	Tokenize(sentence, Words, &nWords);
	strcpy(translated_text, "");
	
	entrylist_count = GetEntryCount(entrylist);
	for(i = 0; i < nWords; i++){
		translation_found = 0;
		for(j = 0; j < entrylist_count; j++){
			for(k = 0; k < entrylist[j].paircount; k++){
				if(strcmp(source_language, entrylist[j].LanguageSet[k].language) == 0 &&
				strcmp(Words[i], entrylist[j].LanguageSet[k].translation) == 0){
					for(m = 0; m < entrylist[j].paircount; m++) {
                        if (strcmp(entrylist[j].LanguageSet[m].language, output_language) == 0) {
                            strcat(translated_text, entrylist[j].LanguageSet[m].translation);
                            strcat(translated_text, " ");
                            translation_found = 1;
                            m = entrylist[j].paircount;
                            k = entrylist[j].paircount;
                            j = entrylist_count;
                        }
                	}
				}
			}
		}
		if (!translation_found) {
            strcat(translated_text, Words[i]);
            strcat(translated_text, " ");
        }
	}
	printf("Sentence:\n%s\n", sentence);
	printf("Sentence translated in %s:\n", output_language);	
	printf("%s\n", translated_text);
}