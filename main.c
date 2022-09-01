#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct heapElement{
    char alphabet;
    int frequency;
    struct heapElement *left;
    struct heapElement *right;
}   heapElement;

typedef struct heap{
    heapElement **arr;
    int heapSize;
    int arraySize;
}   heap;

typedef struct encodes{
    char alphabet;
    char *code;
}   encodes;

typedef struct encodesList{
    encodes *list;
    int currentSize;
}   encodesList;

encodesList* initialiseEncodesList(int size){
    encodesList *n = (encodesList*) malloc(sizeof(encodesList));
    n ->currentSize = 0;
    n->list = (encodes*) malloc(size*sizeof(encodes));
    return n;
}

encodes* addEncodedArrayElement(char character, char* code, int i){
    encodes *newEncodedArrayElement = (encodes*) malloc(sizeof(encodes));
    char* newCode = (char*) malloc(sizeof(char)*i);
    for (int j = 0; j < i; j++){
        newCode[j] = code[j];
    }
    newEncodedArrayElement->code = newCode;
    newEncodedArrayElement->alphabet = character;
    return newEncodedArrayElement;
}

heapElement* addHeapElement(char alphabet, int frequency){
    heapElement *local = (heapElement *)malloc(sizeof(heapElement));
    local->alphabet = alphabet;
    local->frequency = frequency;
    local->left = NULL;
    local->right = NULL;
    return local;
}

heap* initialisePriorityQeue(int size){
    heap *n = (heap*) malloc(sizeof(heap));
    n ->arraySize = size;
    n->heapSize = 0;
    n -> arr = (heapElement**) malloc((size+1)*sizeof(heapElement));
    return n;
}

void insertQeue(heap* node, heapElement *element){
    node -> arr[node->heapSize] = element;
    node ->heapSize = node->heapSize+1;
    int i=node->heapSize;
    while((i>1)&& node->arr[i-1]->frequency < node->arr[((i)/2)-1]->frequency){
        heapElement *temp=node->arr[i-1];
        node->arr[i-1]=node->arr[(i/2)-1];
        node->arr[(i/2)-1]=temp;
        i = i/2;
    }
}

void heapify(heap* node, int i){
    int l = 2*i;
    int r = 2*i+1;
    int largest = i;
    if(l<=node->heapSize && node->arr[l-1]->frequency<node->arr[largest-1]->frequency){
        largest = l;
    }
    if(r<=node->heapSize && node->arr[r-1]->frequency<node->arr[largest-1]->frequency){
        largest = r;
    }
    if(largest!=i){
        heapElement* temp=node->arr[i-1];
        node->arr[i-1]=node->arr[largest-1];
        node->arr[largest-1]=temp;
        heapify(node, largest);
    }
}
heapElement* extractMin(heap* node){
    heapElement* temp=node->arr[0];
    node->arr[0]=node->arr[node->heapSize-1];
    node->arr[node->heapSize-1]=temp;
    node->heapSize=node->heapSize-1;
    heapify(node, 1);
    return temp;
}

void storeCodes(heapElement* root, encodesList* characters,char code[] ,int i){
    if(root->left!=NULL){
        code[i]='0';
        storeCodes(root->left, characters, code, i+1);
    }
    if(root->right!=NULL){
        code[i]='1';
        storeCodes(root->right, characters, code, i+1);
    }
    if(root->left==NULL && root->right==NULL){
        encodes *newEncodedArrayElement = addEncodedArrayElement(root->alphabet, code, i);
        characters->list[characters->currentSize] = *newEncodedArrayElement;
        characters->currentSize = characters->currentSize+1;
    }
}

heapElement* huffmanTree(int charListSize, char* charList, int* freqList){
    heap *priorityQeue;
    heapElement *root, *local, *left, *right;
    priorityQeue = initialisePriorityQeue(charListSize);
    for(int i=0; i<charListSize ; i++){
        local = addHeapElement(charList[i], freqList[i]);
        insertQeue(priorityQeue, local);
    }

    while (priorityQeue->heapSize > 1){
        left = extractMin(priorityQeue);
        right = extractMin(priorityQeue);
        local = addHeapElement(' ', left->frequency + right->frequency);
        local->left = left;
        local->right = right;
        insertQeue(priorityQeue, local);
    }
    root = extractMin(priorityQeue);
    return root;
}

int binaryToInt(int binary){
    int decimal = 0;
    int i = 0;
    while (binary != 0){
        decimal += (binary % 10) * (int) pow(2,i);
        binary = binary/10;
        i++;
    }
    return decimal;
}

void encodingWriting(char* input,char *output){
    FILE *file1;
    FILE *file2;
    int charListSize = 0;
    int num, arrLen, j;
    char *arr, *newArr;
    file1 = fopen(input, "r");
    file2 = fopen(output, "w");
    char c;

    while ((c = fgetc(file1)) != '\n'){
        // charListSize++;
    }
    while((c = fgetc(file1)) != EOF){
        charListSize++;
    }
    fseek(file1, 0, SEEK_SET);
    fprintf(file2, "%d\n", charListSize);
    
    c = fgetc(file1);
    while (c != '\n'){
        fprintf(file2, "%c", c);
        c = fgetc(file1);
    }
    fprintf(file2, "%c", c);
    
    while (c != EOF){
        arr = (char*) malloc(sizeof(char));
        arrLen = 0;
        for (int i = 0; i < 8; i++){
            c = fgetc(file1);
            if (c == EOF){
                break;
            }
            charListSize++;
            arr[i] = c;
            arrLen = arrLen+1;
            arr = (char*) realloc(arr, (arrLen+1)*sizeof(char));
        }
        // charListSize = charListSize % 8;
        num = atoi(arr);
        fprintf(file2, "%c", binaryToInt(num));
    }
        // fseek(file2, 0, SEEK_SET);
        // fprintf(file2,"%d", charListSize);
        // fprintf(file2, "%c", '\n');
        fclose(file1);
        fclose(file2);
}

int encoding(char *input, char *output){
    FILE *fp;
    int *arr = calloc(256, sizeof(int));
    char *charList = (char*) malloc(sizeof(char));
    int charListSize = 0;
    int *freqList = (int*) malloc(sizeof(int));
    int freqListSize = 0;

    fp = fopen(input, "r");

    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0){
        printf("File is empty\n");
        return 0;
    }
    fseek(fp, 0, SEEK_SET);

    char c;
    c = getc(fp);
    while (c != EOF) {
        arr[c] = arr[c] + 1;
        c = getc(fp);
    }    
    fclose(fp);


    for (int i = 0; i < 256; i++) {
        if (arr[i] != 0) {
            charList = (char*) realloc(charList, sizeof(char)*(charListSize+1));
            charList[charListSize] = i;
            charListSize = charListSize+1;
            freqList = (int*) realloc(freqList, sizeof(int)*(freqListSize+1));
            freqList[freqListSize] = arr[i];
            freqListSize = freqListSize+1;
        }
    }

    // for (int i = 0; i < charListSize; i++) {
    //     printf("%c: %d\n", charList[i], freqList[i]);
    // }

    heapElement* root = huffmanTree(charListSize, charList, freqList);

    encodesList *encodesList = initialiseEncodesList(charListSize);
    char code[charListSize];
    storeCodes(root, encodesList, code, 0);
    
    if (charListSize==1){
        encodesList->list[0].code = (char*) malloc(sizeof(char));
        encodesList->list[0].code[0] = '0';
    }

    for(int i=0; i<charListSize; i++){
        printf("%c: %s\n", encodesList->list[i].alphabet, encodesList->list[i].code);
    }
    FILE *fp2;
    fp2 = fopen("intermediate.txt", "w");
    for(int i=0; i<charListSize; i++){
        fprintf(fp2,"%d`%d|", charList[i], freqList[i]);
    }
    fprintf(fp2, "%s", "\n");

    FILE *fp3;
    fp3 = fopen(input, "r");
    c = getc(fp3);
    while (c != EOF) {
        for (int i = 0; i < encodesList->currentSize; i++) {
            if (encodesList->list[i].alphabet == c) {
                fprintf(fp2, "%s", encodesList->list[i].code);
            }
        }
        c = getc(fp3);
    }
    fclose(fp2);
    fclose(fp3);
    encodingWriting("intermediate.txt", output);
    remove("intermediate.txt");
    return 0;
}
char* decimalToBinary(int number){
    char* binary = (char*) malloc(8*sizeof(char));
    for (int i = 0; i < 8; i++){
        binary[i] = '0';
    }
    int i = 1;
    while (number > 0){
        if (number % 2 == 0){
            binary[8-i] = '0';
        }
        else{
            binary[8-i] = '1';
        }
        number = number / 2;
        i++;
    }
    return binary;
}
void decodingWriting(char* input,char *output){
    FILE *file1;
    FILE *file2;
    char *arr;
    char *newArr = (char*) malloc(sizeof(char));
    int newArrLen = 0;
    int c, fileSize;
    file1 = fopen(input, "r");
    file2 = fopen(output, "w");

    c = fgetc(file1);
    while (c != '\n'){
        newArr[newArrLen] = c;
        newArrLen = newArrLen+1;
        newArr = (char*) realloc(newArr, (newArrLen+1)*sizeof(char));
        c = fgetc(file1);
    }
    fileSize = atoi(newArr);

    c = fgetc(file1);
    while (c != '\n'){
        fprintf(file2, "%c", c);
        c = fgetc(file1);
    }
    fprintf(file2, "%c", c);

    for (int i = 0; i < fileSize/8; i++){
        c = fgetc(file1);
        arr = decimalToBinary(c);
        fprintf(file2, "%s", arr);
    }
    c = fgetc(file1);
    arr = decimalToBinary(c);
    for (int i = fileSize%8 ; i > 0; i--){
        fprintf(file2, "%c", arr[8-i]);
    }
    fclose(file1);
    fclose(file2);

}

void decoding(char *input, char *output){
    FILE *fp;
    char c;
    char *charList = (char*) malloc(sizeof(char));
    int charListSize = 0;
    int *freqList = (int*) malloc(sizeof(int));
    int freqListSize = 0;
    int numberSize;
    char *number;
    decodingWriting(input, "intermediate2.txt");
    input = "intermediate2.txt";
    fp = fopen(input, "r");
    c = getc(fp);
    while (c != '\n') {
        number = (char*) malloc(sizeof(char));
        numberSize = 0;
        while(c!='`'){
            number[numberSize] = c;
            numberSize = numberSize+1;
            number = (char*) realloc(number, sizeof(char)*(numberSize+1));
            c = getc(fp);
        }
        charList[charListSize] =(int) atoi(number);
        charListSize = charListSize+1;
        charList = (char*) realloc(charList, sizeof(char)*(charListSize+1));
        // c = getc(fp);

        number = (char*) malloc(sizeof(char));
        numberSize = 0;
        if (c=='`'){
            c = getc(fp);
            while (c != '|') {
                number[numberSize] = c;
                numberSize = numberSize+1;
                number = (char*) realloc(number, sizeof(char)*(numberSize+1));
                c = getc(fp);
            }
            freqList[freqListSize] = (int) atoi(number);
            freqListSize = freqListSize+1;
            freqList = (int*) realloc(freqList, sizeof(int)*(freqListSize+1));
        }
        if (c=='|'){
            c = getc(fp);
        }
    }
    fclose(fp);
    // for (int i = 0; i < charListSize; i++) {
    //     printf("%c: %d\n", charList[i], freqList[i]);
    // }
    int totalChar = 0;
    for (int i = 0; i < freqListSize; i++) {
        totalChar = totalChar + freqList[i];
    }
    heapElement* root = huffmanTree(freqListSize, charList, freqList);
    heapElement* temp = root;

    // now we are supposed to decode the txt seeing the binary code
    fp = fopen(input, "r");
    c = getc(fp);
    while(c!='\n'){
        c = getc(fp);
    }
    c = getc(fp);
    FILE *fp2;
    fp2 = fopen(output, "w");
    while(c!=EOF){
        if (c=='1'){
            temp = temp->right;
        }
        if (c=='0'){
            temp = temp->left;
        }
        if (temp == NULL){
            fprintf(fp2, "%c", root->alphabet);
            temp = root;
            c = getc(fp);
            continue;
        }
        if (temp->left==NULL && temp->right==NULL){
            fprintf(fp2, "%c", temp->alphabet);
            temp = root;
        }
        c = getc(fp);
    }
    fclose(fp2);
    fclose(fp);
    remove("intermediate2.txt");
}

int main(){
    char input[100];
    scanf("%s", input);
    char output[100];
    scanf("%s", output);
    int mode;
    scanf("%d", &mode);
    if(mode==1){
        encoding(input, output);
    }
    else if(mode==2){
        decoding(input,output);
    }

    return 0;
}