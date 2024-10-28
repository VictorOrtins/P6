
def geraTabelaVerdade(m,n): # Função para gerar uma tabela verdade (número de variáveis da tabela(fixo), número de variáveis da tabela(recursivo))
    bits=2**m # determina quantos linhas terá a tabela, valor fixo
    repeticoes_coluna=(bits//(2**n))*2
    repeticoes_linha=(2**n//2)//2
    contador=0 # Esse contador será sempre incrementado até a quantidade de bits e será zerado quando a função repetir
    if not tabelaVerdade: # essa condição cria a primeira coluna da tabela
        for i in range(bits // 2):
            tabelaVerdade.append('0')
        for i in range(bits // 2):
            tabelaVerdade.append('1')
    for j in range(repeticoes_coluna):
        for i in range(repeticoes_linha):
            tabelaVerdade[contador] = tabelaVerdade[contador] + '0'
            contador += 1
        for i in range(repeticoes_linha):
            tabelaVerdade[contador] = tabelaVerdade[contador] + '1'
            contador += 1
    if n==1:
        return tabelaVerdade
    else:
        return geraTabelaVerdade(m,n-1)
    
def formata_tabela(matrizTabelaVerdade):
    for i in range(len(matrizTabelaVerdade)):
        novo = []
        for c in matrizTabelaVerdade[i]:
            if c == '0':
                novo.append(False)
            else:
                novo.append(True)
        matrizTabelaVerdade[i] = novo

def pega_variaveis(expressao):
    split = expressao.split(' ')
    split = [''.join(word.split('(')) for word in split]
    split = [''.join(word.split(')')) for word in split]

    variaveis = []
    for word in split:
        if len(word) == 1:
            if word != '(' and word != ')':
                variaveis.append(word)

    return variaveis

def avalia_expressao(expressao, tabela_verdade, variaveis):
    for index, linha in enumerate(tabela_verdade):
        expressao_copy = str(expressao)
        expressao_copy = expressao_copy.replace('(', '( ')
        expressao_copy = expressao_copy.replace(')', ' )')

        expressao_copy = expressao_copy.split(' ')
        for i, variavel in enumerate(variaveis):
            bool_str = str(linha[i])
            expressao_copy = [bool_str if x == variavel else x for x in expressao_copy]
        expressao_copy = ' '.join(expressao_copy)
        
        tabela_verdade[index] = (linha, eval(expressao_copy))
        
tabelaVerdade = []

expressao = input('Digite a expressão booleana 1(os operandos devem ser OR, AND ou NOT): ')
variaveis_1 = pega_variaveis(expressao)
# print(variaveis_1)

matrizTabelaVerdade = geraTabelaVerdade(len(variaveis_1), len(variaveis_1))
formata_tabela(matrizTabelaVerdade)

avalia_expressao(expressao, matrizTabelaVerdade, variaveis_1)

tabelaVerdade = []

expressao_2 = input('Digite a expressão booleana 2(os operandos devem ser OR, AND ou NOT): ')
variaveis_2 = pega_variaveis(expressao_2)
# print(variaveis_2)

matrizTabelaVerdade_2 = geraTabelaVerdade(len(variaveis_2), len(variaveis_2))
formata_tabela(matrizTabelaVerdade_2)

avalia_expressao(expressao_2, matrizTabelaVerdade_2, variaveis_2)

if set(variaveis_1) == set(variaveis_2):
    print(matrizTabelaVerdade == matrizTabelaVerdade_2)
else:
    print(False)
