#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream>

#define DEBUG 0
#define type long long int

using namespace std;

enum bytecode : char
{
    zero = '0',
    add = '+',
    sub = '-',
    moveNext = '>',
    movePrevious = '<',
    input = ',',
    inputInt = '?',
    output = '.',
    outputInt = ';',
    whileStart = '[',
    whileEnd = ']',
    unknown = '$',
};

char getFirst()
{
    char huge[100];
    scanf("%s", &huge);
    if (huge[0] == '\\' && huge[1] == '0')
        return 0;
    if (huge[0] == '\\' && huge[1] == 'd')
        return -1;
    return huge[0];
}

auto run(bytecode const *instructions)
{
    int i = 0;
    int line = 0;
    unsigned long long int compt = 0;
    const int SIZE = 10000;

    void *labels[255] = {&&end_label};
    labels[bytecode::unknown] = &&end_label;
    labels[bytecode::add] = &&add_label;
    labels[bytecode::input] = &&input_label;
    labels[bytecode::sub] = &&sub_label;
    labels[bytecode::output] = &&output_label;
    labels[bytecode::zero] = &&zero_label;
    labels[bytecode::outputInt] = &&outputInt_label;
    labels[bytecode::movePrevious] = &&movePrevious_label;
    labels[bytecode::moveNext] = &&moveNext_label;
    labels[bytecode::inputInt] = &&inputInt_label;
    labels[bytecode::whileStart] = &&whileStart_label;
    labels[bytecode::whileEnd] = &&whileEnd_label;

    while (labels[instructions[line]] != &&end_label)
    {
        cout << (char)instructions[line++];
    }
    cout << endl;

    int * JumpTable = malloc(sizeof(*JumpTable) * (line+1));
    while (line > 0)
    {
        int indice = line;
        int prog = instructions[line] == ']';
        if (prog)
        {
            prog = -1;
            while ( prog )
            {
                indice--;
                prog += (instructions[indice] == '[') - (instructions[indice] == ']');
            }
            JumpTable[indice] = line;
            JumpTable[line] = indice;
            cout << "JUMP : " << indice <<  " - " << line << endl;
        }
        line--;
    }
    cout << endl;
    line = 0;
    /*
        ['0'] = &&zero_label,
        ['+'] = &&add_label,
        ['-'] = &&sub_label,
        ['>'] = &&moveNext_label,
        ['<'] = &&movePrevious_label,
        [','] = &&input_label,
        ['?'] = &&inputInt_label,
        ['.'] = &&output_label,
        [';'] = &&outputInt_label,
        ['['] = &&whileStart_label,
        [']'] = &&whileEnd_label,
        ['$'] = &&end_label,
    */

    auto const next = [&] {
        // cout << "Currently at line : " << line << " executing : " << (char) instructions[line] << endl;
        // printf("%c\n",instructions[line]);
        compt++;
        return labels[instructions[line++]];
    };

    type *data = calloc(SIZE, sizeof(type));
    goto *next();

zero_label:
    data[i] = 0;
    goto *next();
add_label:
    data[i]++;
    goto *next();
sub_label:
    data[i]--;
    goto *next();
moveNext_label:
    i++;
    goto *next();
movePrevious_label:
    i--;
    goto *next();
input_label:
    data[i] = getFirst();
    goto *next();
inputInt_label:
    int temp;
    scanf("%d", &temp);
    data[i] = temp;
    goto *next();
output_label:
    printf("%c", data[i]);
    goto *next();
outputInt_label:
    printf("%d\n", data[i]);
    goto *next();
whileStart_label:
    if (!data[i])
    {
        line = JumpTable[line-1];
    }
    goto *next();
whileEnd_label:
    if (data[i])
    {
        line = JumpTable[line-1];
    }
    goto *next();
end_label:
    return compt;
}

const char *storage = "+>+>>>?--[-[>+<-]<<<<[>>+<<-]>[<+>>+<-]<[>+<-]>>>>>]<<<;$";
int main()
{
    int T = clock(), Tl = 0;
    auto OP = run((bytecode const *)storage);

    if (clock() == T)
        T--;
    int elapsed = 1 + (1000 * (clock() - T)) / CLOCKS_PER_SEC;
    double perSec = (double)OP / ((double)elapsed / 1000);
    printf("\n%ld op done. in %dms ?\n%.0f per seconds\n\n", OP, elapsed, perSec);
    
    return 0;
}
