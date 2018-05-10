Spirit Tutorial

Gabriel Wernersbach e Eduardo Bezerra

Mar�o 2005

Aplica-se para:

� Visual C++ 2003
� Visual C++ 2005 Beta 1
� ISO-C++ 


O Que � o Spirit ?

http://www.boost.org/libs/spirit/

       Baseada nas mais avan�adas t�cnicas de metaprograma��o, e totalmente implementada em C++ atrav�s do uso de templates, a biblioteca Boost Spirit faz parte do conjunto de bibliotecas que comp�em a Boost Library http://www.boost.org/, e pode ser utilizada no Visual C++ a partir da vers�o 7.1 (Visual Studio 2003).
       
       O Spirit � um framework extremamente vers�til, utilizado para a cria��o de parsers e scanners de todos os tipos: dos mais avan�ados, com gram�ticas complexas e amb�guas, at� os mais simples como, por exemplo, parsers para verifica��o de endere�os de e-mail. O Spirit se posiciona na categoria de parser recursivo-descendente orientado a objetos.
       
       O Spirit pode gerar parsers tanto est�ticos quanto din�micos, utilizando express�es regulares ou apenas definindo regras. Tudo isso diretamente no c�digo C++, n�o havendo a necessidade de executar um pr�-processador ou outra ferramenta para gera��o do c�digo final como, por exemplo, o YACC (Yet Another Compiler Compiler).
       
       O Spirit � um exemplo de DSEL (Domain Specific Embedded Language). Ele disponibiliza uma pequena linguagem que permite definir a sintaxe de linguagens formais. O seu principal dom�nio de abstra��o � chamado �gram�tica livre de contexto.

       Atualmente, a conven��o EBNF (Extended Backus-Naur Form) � uma das mais utilizadas para a defini��o de uma linguagem e sua gram�tica. Por exemplo, se f�ssemos representar em EBNF uma gram�tica que reconhecesse somente espa�os em branco, a mesma poderia ser descrita da seguinte forma:

EBNFEspa�os ::= [ \t\n\r]*;
       No Spirit ter�amos:

SPIRITEspa�os = *space_p;
       Ou

SPIRITEspa�os = *(ch_p(' ')|ch_p('\t')|ch_p('\n')|ch_p('\r'));
       Ou ainda

SPIRITEspa�os = *regex_p(" \t\n\r");
       O Spirit tenta imitar, em C++, da maneira mais pr�xima poss�vel, o formato EBNF. Com isso, facilita o criador da gram�tica na hora de converter a regra de EBNF para uso no pr�prio Spirit.
       
       Em seguida, apresentaremos conceitos b�sicos com os quais devemos nos familiarizar para a utiliza��o do Spirit na pr�tica.


Conceitos B�sicos

Parsers Simples

       Estes s�o exemplos de alguns parsers b�sicos que fazem parte do Spirit framework. Tais parsers constituem os blocos fundamentais com os quais montamos regras e parsers mais complexos.

alpha_pconsome uma letraanychar_pconsome um caractere qualquerdigit_pconsome um d�gito (0 a 9)ch_p(char)consome um caracterestr_p(string)consome uma stringchseq_p(string)consome uma string onde podem existir espa�os em branco entre as letras
NOTA
Consome� medida que o parser avan�a, os caracteres que j� foram reconhecidos s�o consumidos. Da mesma forma, os caracteres n�o reconhecidos s�o recusados, e ditos n�o consumidos.

Operadores Bin�rios

       Definem como regras devem ser verificadas, para juntas comporem um valor. 
       Na descri��o abaixo, R representa uma regra (rule).


LISTA
R1 % R2Encontra um ou mais R1 separados por R2       
       Exemplo:

R1 = ch_p('a');  // consome o caractere �a�
R2 = ch_p(',');  // consome o caractere �,�
R3 = R1 % R2;    // consome uma lista de �a� separado por �,�

       R3 � capaz de ler a string "a,a,a,a,a,a" porque R1 consome os caracteres de valor 'a' e R2 consome os caracteres de valor ','.


DIFEREN�A
R1 - R2Encontra um R1 que n�o seja R2
       Exemplo:

R1 = anychar_p; // consome um caractere qualquer
R2 = digit_p;   // consome um d�gito
R3 = R1 - R2;   // consome qualquer caractere que n�o seja d�gito

       R3 � capaz de reconhecer a string "a" mas n�o reconheceria, por exemplo, a string "2".


SEQU�NCIA
R1 >> R2 ou R1 && R2Consome R1 e R2 em sucess�o
       Exemplo:

R1 = str_p("gera"); // consome a string �gera�
R2 = str_p("l");    // consome o caractere �l�
R3 = R1 >> R2;      // consome somente se as regras aparecerem em ordem
                    // sequencial, primeiro R1 � consumido e, em seguida, R2

       R3 reconhece as strings �geral�, �gera l� ou qualquer �gera� seguido de �l� com qualquer n�mero de espa�os em branco entre os dois (incluindo quebra de linha �\n� �\r�, tabula��o �\t� e espa�o � �).


INTERSE��O
R1 & R2Consome se ambas as regras, R1 e R2, forem v�lidas
       Exemplo:

R1 = ch_p('L')  // consome os caracteres �L� �A� e �B� nessa ordem,
  >> ch_p('A')  // podendo ter espa�os em branco entre elas ou n�o
  >> ch_p('B');

R2 = chseq_p("LAB");  // idem anterior

R3 = R1 & R2;         // sempre v�lido para combina��es de �L� �A� e �B�
                      // pois as duas regras s�o sin�nimas

       R3 reconhece qualquer combina��o dos caracteres �L�, �A� e �B� com espa�os intermediando os caracteres ou n�o.


UNI�O
R1 | R2Consome se uma das regras R1 ou R2 forem v�lidas
       Exemplo:

R1 = ch_p('a'); // consome o caractere �a�
R2 = ch_p('b'); // consome o caractere �b�
R3 = R1 | R2;   // consome somente se for o caractere �a� ou �b�

R3 reconhece �a� ou �b�.

       Numa seq��ncia de A | B | C | D ... | N, a primeira regra a encontrar um valor v�lido faz parar a seq��ncia de busca e retornar o valor. Caso voc� tenha:

R1 = str_p("ser");     // consome a palavra �ser�
R2 = str_p("servi�o"); // consome a palavra �servi�o�
R3 = R1 | R2;          // consome �ser� ou �servi�o�, nessa ordem

       Se R3 encontrar a palavra �servi�o�, vai retornar apenas �ser� e, caso seja feita uma nova busca, o buffer estar� com �vi�o�. Para evitar isso, pode-se utilizar a diretiva longest_d (que for�a o teste de todas as alternativas e retorna a que consumiu mais texto) ou coloca-se sempre a maior regra antes da menor:

R1 = str_p("servi�o");
R2 = str_p("ser");
R3 = R1 | R2;

       Agora R3 identifica corretamente �servi�o� e �ser�. Tamb�m poder�amos inverter a ordem de forma��o de R3, ou seja, R3 = R2 | R1.


OU EXCLUSIVO (XOR)
R1 ^ R2Consome somente se uma das regras, R1 ou R2, forem v�lidas
       Exemplo:

R1 = ch_p('L')  // consome os caracteres �L�, �A� e �B� ,nessa
  >> ch_p('A')  // ordem, podendo ter espa�os em branco entre elas ou n�o
  >> ch_p('B');

R2 = chseq_p("LAB") | chseq_p("MIL");  // chseq_p("LAB") � sin�nimo da regra R1,
                                       // mas chseq_p("MIL") n�o

R3 = R1 ^ R2;  // agora R3 s� consome quando for uma
               // combina��o dos caracteres �M�, �I�, e �L� em seq��ncia

       R3 reconhece apenas os caracteres �M�, �I� e �L� ,com espa�os intermediando as mesmas, ou n�o. No caso de aparecer a combina��o �L�, �A� e �B�, como as duas regras retornam positivo, a regra n�o consome texto.


OU SEQUENCIAL
R1 || R2Encontra R1 | R2 | R1 >> R2 que � o mesmo que encontrar R1 ou R2, ou R1 seguido de R2
       Exemplo:

R1 = ch_p('a'); // consome o caractere �a�
R2 = ch_p('b'); // consome o caractere �b�
R3 = R1 || R2;  // consome as combina��es de R1 com R2 (menos R2 seguido de R1)

       R3 encontra os caracteres �a�, �b� ou �a� seguido de �b�, podendo estar separado por um n�mero indeterminado de espa�os em branco.


Operadores Un�rios

       Definem a aplica��o da regra de acordo com a quantidade de vezes em que a regra deve encontrar um valor. Na descri��o abaixo, R representa uma regra (rule).


OPCIONAL
!RR pode aparecer de zero a uma vez
Exemplo:

R1 = ch_p('a') >> !ch_p('b'); // consome o caractere �a�, seguido ou n�o do caractere �b�

       R1 � capaz de consumir a string �ab�, �a� seguido por �b� ap�s um n�mero indeterminado de espa�os em branco ou �a�, pois, caso �b� n�o apare�a, a regra continua consumindo texto.


KLEENE STAR
*REncontra R zero ou mais vezes
       Exemplo:

R1 = *ch_p('b'); // consome �b� at� consumir todos os �b� dispon�veis
                 // retorna sempre v�lido, mesmo n�o tendo consumido nenhum �b�

R2 = ch_p('a') >> R1 >> ch_p('c');

       R2 pode reconhecer qualquer seq��ncia onde �a� � seguido de um n�mero indeterminado de �b�, seguido de �c�.
       
       Por exemplo, essa regra poderia reconhecer:

* ac
* abc
* abbc
* abbbbbbbbbbbbbbbbbbbbbbbbbc
* a c
* a b c
* a b b c
* a bbbbbbbbbbbbbbbbbbbbbbbbb c


POSITIVO
+RBusca por R de uma a mais vezes
       Exemplo:

R1 = +ch_p('b');  // consome �b� at� consumir todos os �b� dispon�veis
                  // retorna v�lido somente se tiver consumido pelo menos um �b�

R2 = ch_p('a') >> R1 >> ch_p('c');

       R2 pode reconhecer qualquer seq��ncia onde �a� � seguido de 1 a um n�mero indeterminado N de �b�, seguido de �c�.

       Por exemplo, essa regra poderia reconhecer:

* abc
* abbc
* abbbbbbbbbbbbbbbbbbbbbbbbbc
* a b c
* a b b c
* a bbbbbbbbbbbbbbbbbbbbbbbbb c


NEGATIVO
~RBusca por uma entidade que n�o seja R. Deve ser usado somente para parsers de caractere
       Exemplo:

R1 = ~ch_p('x');

       R1 consome apenas os caracteres que forem diferentes de �x�.


Regras

       A regra representa um parser polim�rfico capaz de, por si s�, servir de container para a express�o associada a ele. As regras servem de atalho para facilitar o entendimento de express�es complexas e podem ser alteradas utilizando-se diretivas que mudem o seu comportamento padr�o.
       
       As regras s�o diferentes de qualquer outro objeto C++, pois apresentam caracter�sticas �nicas. Elas n�o possuem as sem�nticas de c�pia e atribui��o e, sendo assim, n�o podem ser armazenadas em containers, n�o podem ser passadas ou retornadas de fun��es por valor.
       
       Como exemplo, poder�amos definir as seguintes regras:

rule<> rule_1 = *(A | B) & +(C | D | E); // onde �A�, �B�, �C�, �D� e �E�
                                         // s�o parsers ou outras regras

rule<> rule_2 = f >> g >> h >> rule_1;

       Sempre que for necess�rio fazer c�pia de uma regra, deve-se usar regras din�micas. Tais regras s�o representadas pela classe stored_rule<> que possui as sem�nticas de c�pia e atribui��o devidamente implementadas.
       
       O exemplo acima poderia ser reescrito utilizando-se regras din�micas da seguinte forma:

stored_rule<> rule_1 = *(A | B) & +(C | D | E); // onde �A�, �B�, �C�, �D� e �E�
                                                // s�o parsers ou outras regras

stored_rule<> rule_2 = f >> g >> h >> rule_1;

       Como se pode notar, a interface de utiliza��o � exatamente igual a da classe rule<>.
       
       Regras podem ser recursivas, e seguem o fluxo da express�o, por exemplo:

A = A | B; // n�o ir� consumir texto e ficar� infinitamente
           // entrando na regra 'A' at� exaurir o limite de recurs�o

       Isso � o que chamamos de �Recurs�o � Esquerda� e, se a mesma n�o for evitada, a sua aplica��o apresentar� erros durante o runtime.

       Se definirmos a seguinte regra:

A = B | A >> C >> B;

       Ela ficar� infinitamente chamando a si mesma caso n�o encontre �B�, por�m, se alterarmos a mesma para:

A = B >> *(C >> B)

       O problema de recurs�o deixar� de existir.
       
       Esse assunto � abordado em detalhes no FAQ do Spirit em http://www.boost.org/libs/spirit/doc/faq.html, sobre o t�pico �Eliminating Left Recursion�.


Diretivas

       As diretivas auxiliam na mudan�a de comportamento do Scanner padr�o, o qual � respons�vel pelo avan�o de caractere e an�lise do conte�do. Dentro das principais diretivas existentes no Spirit, encontraremos a diretiva lexeme_d.

lexeme_dMuda o comportamento de avan�o autom�tico de espa�os em branco

R1 = lexeme_d[ ch_p('L')  // consome os caracteres �L�, �A� e �B�,nessa ordem,
            >> ch_p('A')  // n�o podendo existir espa�os em branco entre elas
            >> ch_p('B') ];

R2 = chseq_p("LAB");  // consome os caracteres L�, �A� e �B�,nessa ordem,
                      // podendo existir espa�os em branco entre elas

R3 = R1 & R2;  // v�lido somente para �LAB�, pois � a �nica
               // seq��ncia que as duas regras consomem

       A diretiva lexeme_d s� pode ser usada com parsers de caractere como, por exemplo, ch_p, anychar_p, digit_p, entre outros. Sempre que for necess�rio utilizar uma regra dentro de uma diretiva lexeme_d, devemos ajustar o tipo do scanner para scanner m�ltiplo da seguinte forma:

// � necess�rio definir o n�mero de scanners que a lista suporta
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 2

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/symbols.hpp>

// Define um novo tipo que suporta uma regra dentro de uma diretiva �lexeme_d�
typedef scanner_list<ScannerT, typename lexeme_scanner<ScannerT>::type> LexdScanner;

// Essa regra pode estar dentro de uma diretiva �lexeme_d�
rule<LexdScanner> LexdRule;

R1 = lexeme_d[ digit_p >> LexdRule];
LexdRule = str_p(",00");


A��es Sem�nticas

       As a��es sem�nticas servem para executar a��es diretamente durante a an�lise feita pelas regras. Elas s�o compat�veis e podem ser utilizadas com fun��es (free-functions) e objetos-fun��o (functors). A��es sem�nticas podem ser associadas a qualquer parser e podem estar presentes em qualquer ponto da especifica��o de uma gram�tica. O uso de a��es sem�nticas � recomendado para gram�ticas simples que n�o apresentem ambig�idade.
       
       Embora simplifique o tratamento de uma gram�tica, se a mesma contiver ambig�idades (ou seja, se mais de uma regra entender o mesmo trecho de input, diferenciando somente ap�s algumas regras), as a��es precisar�o ser desfeitas.
       Para os casos em que as gram�ticas s�o complexas e amb�guas, o Spirit oferece como solu��o o uso de Parse Tree e Abstract Syntax Tree (AST). 
       Este t�pico � abordado com detalhes em http://www.boost.org/libs/spirit/doc/trees.html.

Utilizando Fun��es (Free Functions)

       Podemos ter a��es recebendo qualquer tipo de valor vindo de uma gram�tica, como, por exemplo:

void PrintString(char const* str, char const* end)
{
    std::string s(str, end);
    std::cout << s;
}
R1 = str_p("palavra-chave")[&PrintString];

R1 ir� imprimir na tela, via a��o sem�ntica (invocando a fun��o print_string), a string "palavra-chave".

       A assinatura das fun��es que trabalham com a��es sem�nticas mudam de acordo com o tipo de parser que as invoca. Para uma regra que l� apenas caracteres, a assinatura da fun��o seria a seguinte:

void PrintChar(char const s)
{
    std::cout << s;
}
R1 = ch_p('C')[&PrintChar];

       R2 ir� imprimir na tela, via a��o sem�ntica (invocando a fun��o print_char),o caracter 'C'.

Utilizando Objetos-Fun��o (Functors)

       A��es sem�nticas tamb�m podem ser implementadas atrav�s da utiliza��o de functors. O exemplo a seguir demonstra essa possibilidade:

struct PrintString
{
    void operator(char const* Str, char const* End) () const
    {
        std::cout << std::string(Str, End);
    }
};
R1 = str_p("palavra-chave")[PrintString];

       R1 ir� imprimir na tela, via a��o sem�ntica (invocando o objeto-fun��o PrintString), a string "palavra-chave".


A Gram�tica

       A gram�tica serve como uma esp�cie de dep�sito de regras. Na gram�tica, as regras podem ser definidas e pode-se, inclusive, utilizar outras gram�ticas internamente como se fossem regras.
       
       Toda gram�tica a ser implementada deve seguir o padr�o descrito abaixo:

struct RealGrammar : public grammar<RealGrammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(RealGrammar const& /*self*/)
        {
            // Corpo da regra. Lista de n�meros delimitados por v�rgula
            rule_ = real_p >> *(ch_p(',') >> real_p);
        }

        // Defini��o da regra
        rule<ScannerT> rule_;

        // Ponto de entrada onde o fluxo de regras come�a a consumir o texto
        rule<ScannerT> const& start() const
        {        
            // Regra inicial
            return rule_;
        }
    };
};

       A classe que ir� implementar a gram�tica deve derivar publicamente da classe template grammar<>, e utilizar o construtor da classe interna definition para definir as suas regras. Essa implementa��o faz uso da t�cnica conhecida com CRTP (Curriously Recurring Template Pattern) , muito utilizada, por exemplo, por bibliotecas como a ATL (Active Template Library).
       
       A partir desse ponto, a gram�tica est� pronta para ser utilizada, bastando instanciar um objeto do tipo definido e executar a fun��o parse, passando como par�metros: a string de input a ser parseada, a inst�ncia da gram�tica previamente definida e a regra de avan�o dentro da string que se deseja analisar.

std::string input = "10, 21 369, 642";
RealGrammar grammar;
                                      // �space_p� ignora espa�os e tabula��es
parse_info<> info = parse(input, grammar, space_p); 

if (info.full()) // Verifica se o input foi totalmente parseado
    std::cout << "Conte�do parseado com sucesso";

       A fun��o parse retorna, entre outras coisas, o resultado do parseamento atrav�s do tipo parse_info<>. Caso o parser tenha totalmente parseado o input fornecido, a fun��o-membro parse_info<>::full() ir� retornar verdadeiro.

Boost::function e Gram�ticas Com  A��es Sem�nticas 

       No caso de desejar utilizar a��es sem�nticas em conjunto com fun��es-membro da classe que define a nossa gram�tica, podemos recorrer a boost::function. Atrav�s da utiliza��o de boost::function podemos fazer com que fun��es-membro sejam chamadas durante o processo de an�lise gramatical.
       
       Por exemplo, reproduzindo a gram�tica anteriormente definida, ter�amos:

struct RealGrammar : public grammar<RealGrammar>
{
    RealGrammar()
    {
        action_ = boost::bind(&RealGrammar::Action, this);
    }
    template <typename ScannerT>
    struct definition
    {
        definition(RealGrammar const& self)
        {
            rule_ = (real_p >> *(ch_p(',') >> real_p))[ self.action_ ];
        }

        rule<ScannerT> rule_;

        rule<ScannerT> const& start() const
        {        
            return rule_;
        }
    };
private:
    void Action(const char* First, const char* Last) const
    {
        std::cout << std::string(First, Last);
    }
    boost::function<void (const char*, const char*)> action_;
};

       Os elementos destacados em azul e negrito servem para indicar os pontos de altera��o mais relevantes no c�digo.
       
       Come�amos definindo uma vari�vel-membro action_ do tipo boost::function com uma assinatura compat�vel com a da fun��o-membro desejada Action.
       
       A seguir, no construtor da classe RealGrammar, inicializamos action_ para executar a fun��o-membro Action quando disparada por uma a��o sem�ntica. Essa associa��o � feita atrav�s de boost::bind, que recebe como par�metros o endere�o da fun��o-membro &RealGrammar::Action e o ponteiro this.
       
       Na defini��o da regra rule_ associamos a a��o sem�ntica a self.action_ que atua como um delegate que executa a fun��o-membro desejada.
       
       Com a atua��o conjunta de boost::function e boost::bind � poss�vel obter a funcionalidade desejada, ou seja, utilizar-se fun��es-membro como par�metros de a��es sem�nticas.


Sum�rio

       Este tutorial teve com objetivo introduzir o Spirit framework aos programadores C++ que fazem utiliza��o do Visual C++ como a sua principal ferramenta de desenvolvimento no dia-a-dia.
       
       A escalabilidade e robustez do Spirit permite escrever de parser simples e triviais a parsers com gram�ticas extremamente complexas e amb�guas, com uma sintaxe que se aproxima ao m�ximo de EBNF, sem abandonar os conceitos e a expressividade de C++ .
       
       O Spirit representa, atualmente, em conjunto com a Boost Library, o que existe de mais moderno em t�cnicas de desenvolvimento em C++, tais como: Meta-Programming, Policy Classes e Expression Templates.
       

Exemplos

       Junto com o artigo voc� encontrar� dois projetos em Visual C++ 2003 e respectivos makefiles (caso deseje trabalhar a partir da linha de comando). Esses projetos fazem refer�ncia � vari�vel de ambiente $(BOOST) que deve estar definida no sistema e apontar para o diret�rio onde se encontra instalada a Boost Library.
       

Sobre os Autores

       Eduardo Bezerra, Engenheiro s�nior desenvolvedor de software, especializado em C++, com mais de 19 anos de experi�ncia pr�tica. Atua em projetos nas plataformas .NET / Windows e UNIX. Possui a certifica��o Microsoft Certified Application Developer (MCAD) Charter Member. � co-autor do livro C++Builder Developer's Guide, SAM'S Publishing Co.
       Gabriel Wernersbach, Analista de Sistemas, especializado em C e C++, com mais de 9 anos de experi�ncia pr�tica. Atua em projetos nas plataformas .NET / Windows e Unix. Possui grande experi�ncia na migra��o de software entre sistemas operacionais e atualmente � respons�vel pela manuten��o de v�rios sistemas, incluindo uma interface SQL ANSI feita exclusivamente com Spirit de sua pr�pria autoria.

Sobre o Artigo
       
       Este artigo � propriedade intelectual de Gabriel Wernersbach e Eduardo Bezerra, cedido apenas para o MSDN Brasil (www.microsoft.com/brasil/msdn) e seus usu�rios. � expressamente proibida a reprodu��o deste artigo por terceiros, sem autoriza��o formal do autor, como inclus�o em livros de outros autores e sites n�o autorizados.

