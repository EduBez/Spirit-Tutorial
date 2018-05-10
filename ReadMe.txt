Spirit Tutorial

Gabriel Wernersbach e Eduardo Bezerra

Março 2005

Aplica-se para:

• Visual C++ 2003
• Visual C++ 2005 Beta 1
• ISO-C++ 


O Que é o Spirit ?

http://www.boost.org/libs/spirit/

       Baseada nas mais avançadas técnicas de metaprogramação, e totalmente implementada em C++ através do uso de templates, a biblioteca Boost Spirit faz parte do conjunto de bibliotecas que compõem a Boost Library http://www.boost.org/, e pode ser utilizada no Visual C++ a partir da versão 7.1 (Visual Studio 2003).
       
       O Spirit é um framework extremamente versátil, utilizado para a criação de parsers e scanners de todos os tipos: dos mais avançados, com gramáticas complexas e ambíguas, até os mais simples como, por exemplo, parsers para verificação de endereços de e-mail. O Spirit se posiciona na categoria de parser recursivo-descendente orientado a objetos.
       
       O Spirit pode gerar parsers tanto estáticos quanto dinâmicos, utilizando expressões regulares ou apenas definindo regras. Tudo isso diretamente no código C++, não havendo a necessidade de executar um pré-processador ou outra ferramenta para geração do código final como, por exemplo, o YACC (Yet Another Compiler Compiler).
       
       O Spirit é um exemplo de DSEL (Domain Specific Embedded Language). Ele disponibiliza uma pequena linguagem que permite definir a sintaxe de linguagens formais. O seu principal domínio de abstração é chamado “gramática livre de contexto.

       Atualmente, a convenção EBNF (Extended Backus-Naur Form) é uma das mais utilizadas para a definição de uma linguagem e sua gramática. Por exemplo, se fôssemos representar em EBNF uma gramática que reconhecesse somente espaços em branco, a mesma poderia ser descrita da seguinte forma:

EBNFEspaços ::= [ \t\n\r]*;
       No Spirit teríamos:

SPIRITEspaços = *space_p;
       Ou

SPIRITEspaços = *(ch_p(' ')|ch_p('\t')|ch_p('\n')|ch_p('\r'));
       Ou ainda

SPIRITEspaços = *regex_p(" \t\n\r");
       O Spirit tenta imitar, em C++, da maneira mais próxima possível, o formato EBNF. Com isso, facilita o criador da gramática na hora de converter a regra de EBNF para uso no próprio Spirit.
       
       Em seguida, apresentaremos conceitos básicos com os quais devemos nos familiarizar para a utilização do Spirit na prática.


Conceitos Básicos

Parsers Simples

       Estes são exemplos de alguns parsers básicos que fazem parte do Spirit framework. Tais parsers constituem os blocos fundamentais com os quais montamos regras e parsers mais complexos.

alpha_pconsome uma letraanychar_pconsome um caractere qualquerdigit_pconsome um dígito (0 a 9)ch_p(char)consome um caracterestr_p(string)consome uma stringchseq_p(string)consome uma string onde podem existir espaços em branco entre as letras
NOTA
ConsomeÀ medida que o parser avança, os caracteres que já foram reconhecidos são consumidos. Da mesma forma, os caracteres não reconhecidos são recusados, e ditos não consumidos.

Operadores Binários

       Definem como regras devem ser verificadas, para juntas comporem um valor. 
       Na descrição abaixo, R representa uma regra (rule).


LISTA
R1 % R2Encontra um ou mais R1 separados por R2       
       Exemplo:

R1 = ch_p('a');  // consome o caractere ‘a’
R2 = ch_p(',');  // consome o caractere ‘,’
R3 = R1 % R2;    // consome uma lista de ‘a’ separado por ‘,’

       R3 é capaz de ler a string "a,a,a,a,a,a" porque R1 consome os caracteres de valor 'a' e R2 consome os caracteres de valor ','.


DIFERENÇA
R1 - R2Encontra um R1 que não seja R2
       Exemplo:

R1 = anychar_p; // consome um caractere qualquer
R2 = digit_p;   // consome um dígito
R3 = R1 - R2;   // consome qualquer caractere que não seja dígito

       R3 é capaz de reconhecer a string "a" mas não reconheceria, por exemplo, a string "2".


SEQUÊNCIA
R1 >> R2 ou R1 && R2Consome R1 e R2 em sucessão
       Exemplo:

R1 = str_p("gera"); // consome a string “gera”
R2 = str_p("l");    // consome o caractere ‘l’
R3 = R1 >> R2;      // consome somente se as regras aparecerem em ordem
                    // sequencial, primeiro R1 é consumido e, em seguida, R2

       R3 reconhece as strings “geral”, “gera l” ou qualquer “gera” seguido de “l” com qualquer número de espaços em branco entre os dois (incluindo quebra de linha ‘\n’ ‘\r’, tabulação ‘\t’ e espaço ‘ ’).


INTERSEÇÃO
R1 & R2Consome se ambas as regras, R1 e R2, forem válidas
       Exemplo:

R1 = ch_p('L')  // consome os caracteres ‘L’ ‘A’ e ‘B’ nessa ordem,
  >> ch_p('A')  // podendo ter espaços em branco entre elas ou não
  >> ch_p('B');

R2 = chseq_p("LAB");  // idem anterior

R3 = R1 & R2;         // sempre válido para combinações de ‘L’ ‘A’ e ‘B’
                      // pois as duas regras são sinônimas

       R3 reconhece qualquer combinação dos caracteres ‘L’, ‘A’ e ‘B’ com espaços intermediando os caracteres ou não.


UNIÃO
R1 | R2Consome se uma das regras R1 ou R2 forem válidas
       Exemplo:

R1 = ch_p('a'); // consome o caractere ‘a’
R2 = ch_p('b'); // consome o caractere ‘b’
R3 = R1 | R2;   // consome somente se for o caractere ‘a’ ou ‘b’

R3 reconhece ‘a’ ou ’b’.

       Numa seqüência de A | B | C | D ... | N, a primeira regra a encontrar um valor válido faz parar a seqüência de busca e retornar o valor. Caso você tenha:

R1 = str_p("ser");     // consome a palavra “ser”
R2 = str_p("serviço"); // consome a palavra “serviço”
R3 = R1 | R2;          // consome “ser” ou “serviço”, nessa ordem

       Se R3 encontrar a palavra “serviço”, vai retornar apenas “ser” e, caso seja feita uma nova busca, o buffer estará com “viço”. Para evitar isso, pode-se utilizar a diretiva longest_d (que força o teste de todas as alternativas e retorna a que consumiu mais texto) ou coloca-se sempre a maior regra antes da menor:

R1 = str_p("serviço");
R2 = str_p("ser");
R3 = R1 | R2;

       Agora R3 identifica corretamente “serviço” e “ser”. Também poderíamos inverter a ordem de formação de R3, ou seja, R3 = R2 | R1.


OU EXCLUSIVO (XOR)
R1 ^ R2Consome somente se uma das regras, R1 ou R2, forem válidas
       Exemplo:

R1 = ch_p('L')  // consome os caracteres ‘L’, ‘A’ e ‘B’ ,nessa
  >> ch_p('A')  // ordem, podendo ter espaços em branco entre elas ou não
  >> ch_p('B');

R2 = chseq_p("LAB") | chseq_p("MIL");  // chseq_p("LAB") é sinônimo da regra R1,
                                       // mas chseq_p("MIL") não

R3 = R1 ^ R2;  // agora R3 só consome quando for uma
               // combinação dos caracteres ‘M’, ’I’, e ‘L’ em seqüência

       R3 reconhece apenas os caracteres ‘M’, ’I’ e ‘L’ ,com espaços intermediando as mesmas, ou não. No caso de aparecer a combinação ‘L’, ‘A’ e ‘B’, como as duas regras retornam positivo, a regra não consome texto.


OU SEQUENCIAL
R1 || R2Encontra R1 | R2 | R1 >> R2 que é o mesmo que encontrar R1 ou R2, ou R1 seguido de R2
       Exemplo:

R1 = ch_p('a'); // consome o caractere ‘a’
R2 = ch_p('b'); // consome o caractere ‘b’
R3 = R1 || R2;  // consome as combinações de R1 com R2 (menos R2 seguido de R1)

       R3 encontra os caracteres ‘a’, ‘b’ ou ‘a’ seguido de ‘b’, podendo estar separado por um número indeterminado de espaços em branco.


Operadores Unários

       Definem a aplicação da regra de acordo com a quantidade de vezes em que a regra deve encontrar um valor. Na descrição abaixo, R representa uma regra (rule).


OPCIONAL
!RR pode aparecer de zero a uma vez
Exemplo:

R1 = ch_p('a') >> !ch_p('b'); // consome o caractere ‘a’, seguido ou não do caractere ‘b’

       R1 é capaz de consumir a string ”ab”, ‘a’ seguido por ‘b’ após um número indeterminado de espaços em branco ou ‘a’, pois, caso ‘b’ não apareça, a regra continua consumindo texto.


KLEENE STAR
*REncontra R zero ou mais vezes
       Exemplo:

R1 = *ch_p('b'); // consome ‘b’ até consumir todos os ‘b’ disponíveis
                 // retorna sempre válido, mesmo não tendo consumido nenhum ‘b’

R2 = ch_p('a') >> R1 >> ch_p('c');

       R2 pode reconhecer qualquer seqüência onde ‘a’ é seguido de um número indeterminado de ‘b’, seguido de ‘c’.
       
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

R1 = +ch_p('b');  // consome ‘b’ até consumir todos os ‘b’ disponíveis
                  // retorna válido somente se tiver consumido pelo menos um ‘b’

R2 = ch_p('a') >> R1 >> ch_p('c');

       R2 pode reconhecer qualquer seqüência onde ‘a’ é seguido de 1 a um número indeterminado N de ‘b’, seguido de ‘c’.

       Por exemplo, essa regra poderia reconhecer:

* abc
* abbc
* abbbbbbbbbbbbbbbbbbbbbbbbbc
* a b c
* a b b c
* a bbbbbbbbbbbbbbbbbbbbbbbbb c


NEGATIVO
~RBusca por uma entidade que não seja R. Deve ser usado somente para parsers de caractere
       Exemplo:

R1 = ~ch_p('x');

       R1 consome apenas os caracteres que forem diferentes de ‘x’.


Regras

       A regra representa um parser polimórfico capaz de, por si só, servir de container para a expressão associada a ele. As regras servem de atalho para facilitar o entendimento de expressões complexas e podem ser alteradas utilizando-se diretivas que mudem o seu comportamento padrão.
       
       As regras são diferentes de qualquer outro objeto C++, pois apresentam características únicas. Elas não possuem as semânticas de cópia e atribuição e, sendo assim, não podem ser armazenadas em containers, não podem ser passadas ou retornadas de funções por valor.
       
       Como exemplo, poderíamos definir as seguintes regras:

rule<> rule_1 = *(A | B) & +(C | D | E); // onde ‘A’, ‘B’, ‘C’, ‘D’ e ‘E’
                                         // são parsers ou outras regras

rule<> rule_2 = f >> g >> h >> rule_1;

       Sempre que for necessário fazer cópia de uma regra, deve-se usar regras dinâmicas. Tais regras são representadas pela classe stored_rule<> que possui as semânticas de cópia e atribuição devidamente implementadas.
       
       O exemplo acima poderia ser reescrito utilizando-se regras dinâmicas da seguinte forma:

stored_rule<> rule_1 = *(A | B) & +(C | D | E); // onde ‘A’, ‘B’, ‘C’, ‘D’ e ‘E’
                                                // são parsers ou outras regras

stored_rule<> rule_2 = f >> g >> h >> rule_1;

       Como se pode notar, a interface de utilização é exatamente igual a da classe rule<>.
       
       Regras podem ser recursivas, e seguem o fluxo da expressão, por exemplo:

A = A | B; // não irá consumir texto e ficará infinitamente
           // entrando na regra 'A' até exaurir o limite de recursão

       Isso é o que chamamos de “Recursão à Esquerda” e, se a mesma não for evitada, a sua aplicação apresentará erros durante o runtime.

       Se definirmos a seguinte regra:

A = B | A >> C >> B;

       Ela ficará infinitamente chamando a si mesma caso não encontre ‘B’, porém, se alterarmos a mesma para:

A = B >> *(C >> B)

       O problema de recursão deixará de existir.
       
       Esse assunto é abordado em detalhes no FAQ do Spirit em http://www.boost.org/libs/spirit/doc/faq.html, sobre o tópico “Eliminating Left Recursion”.


Diretivas

       As diretivas auxiliam na mudança de comportamento do Scanner padrão, o qual é responsável pelo avanço de caractere e análise do conteúdo. Dentro das principais diretivas existentes no Spirit, encontraremos a diretiva lexeme_d.

lexeme_dMuda o comportamento de avanço automático de espaços em branco

R1 = lexeme_d[ ch_p('L')  // consome os caracteres ‘L’, ‘A’ e ‘B’,nessa ordem,
            >> ch_p('A')  // não podendo existir espaços em branco entre elas
            >> ch_p('B') ];

R2 = chseq_p("LAB");  // consome os caracteres L’, ‘A’ e ‘B’,nessa ordem,
                      // podendo existir espaços em branco entre elas

R3 = R1 & R2;  // válido somente para “LAB”, pois é a única
               // seqüência que as duas regras consomem

       A diretiva lexeme_d só pode ser usada com parsers de caractere como, por exemplo, ch_p, anychar_p, digit_p, entre outros. Sempre que for necessário utilizar uma regra dentro de uma diretiva lexeme_d, devemos ajustar o tipo do scanner para scanner múltiplo da seguinte forma:

// É necessário definir o número de scanners que a lista suporta
#define BOOST_SPIRIT_RULE_SCANNERTYPE_LIMIT 2

#include <boost/spirit/core.hpp>
#include <boost/spirit/utility.hpp>
#include <boost/spirit/symbols.hpp>

// Define um novo tipo que suporta uma regra dentro de uma diretiva ‘lexeme_d’
typedef scanner_list<ScannerT, typename lexeme_scanner<ScannerT>::type> LexdScanner;

// Essa regra pode estar dentro de uma diretiva ‘lexeme_d’
rule<LexdScanner> LexdRule;

R1 = lexeme_d[ digit_p >> LexdRule];
LexdRule = str_p(",00");


Ações Semânticas

       As ações semânticas servem para executar ações diretamente durante a análise feita pelas regras. Elas são compatíveis e podem ser utilizadas com funções (free-functions) e objetos-função (functors). Ações semânticas podem ser associadas a qualquer parser e podem estar presentes em qualquer ponto da especificação de uma gramática. O uso de ações semânticas é recomendado para gramáticas simples que não apresentem ambigüidade.
       
       Embora simplifique o tratamento de uma gramática, se a mesma contiver ambigüidades (ou seja, se mais de uma regra entender o mesmo trecho de input, diferenciando somente após algumas regras), as ações precisarão ser desfeitas.
       Para os casos em que as gramáticas são complexas e ambíguas, o Spirit oferece como solução o uso de Parse Tree e Abstract Syntax Tree (AST). 
       Este tópico é abordado com detalhes em http://www.boost.org/libs/spirit/doc/trees.html.

Utilizando Funções (Free Functions)

       Podemos ter ações recebendo qualquer tipo de valor vindo de uma gramática, como, por exemplo:

void PrintString(char const* str, char const* end)
{
    std::string s(str, end);
    std::cout << s;
}
R1 = str_p("palavra-chave")[&PrintString];

R1 irá imprimir na tela, via ação semântica (invocando a função print_string), a string "palavra-chave".

       A assinatura das funções que trabalham com ações semânticas mudam de acordo com o tipo de parser que as invoca. Para uma regra que lê apenas caracteres, a assinatura da função seria a seguinte:

void PrintChar(char const s)
{
    std::cout << s;
}
R1 = ch_p('C')[&PrintChar];

       R2 irá imprimir na tela, via ação semântica (invocando a função print_char),o caracter 'C'.

Utilizando Objetos-Função (Functors)

       Ações semânticas também podem ser implementadas através da utilização de functors. O exemplo a seguir demonstra essa possibilidade:

struct PrintString
{
    void operator(char const* Str, char const* End) () const
    {
        std::cout << std::string(Str, End);
    }
};
R1 = str_p("palavra-chave")[PrintString];

       R1 irá imprimir na tela, via ação semântica (invocando o objeto-função PrintString), a string "palavra-chave".


A Gramática

       A gramática serve como uma espécie de depósito de regras. Na gramática, as regras podem ser definidas e pode-se, inclusive, utilizar outras gramáticas internamente como se fossem regras.
       
       Toda gramática a ser implementada deve seguir o padrão descrito abaixo:

struct RealGrammar : public grammar<RealGrammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(RealGrammar const& /*self*/)
        {
            // Corpo da regra. Lista de números delimitados por vírgula
            rule_ = real_p >> *(ch_p(',') >> real_p);
        }

        // Definição da regra
        rule<ScannerT> rule_;

        // Ponto de entrada onde o fluxo de regras começa a consumir o texto
        rule<ScannerT> const& start() const
        {        
            // Regra inicial
            return rule_;
        }
    };
};

       A classe que irá implementar a gramática deve derivar publicamente da classe template grammar<>, e utilizar o construtor da classe interna definition para definir as suas regras. Essa implementação faz uso da técnica conhecida com CRTP (Curriously Recurring Template Pattern) , muito utilizada, por exemplo, por bibliotecas como a ATL (Active Template Library).
       
       A partir desse ponto, a gramática está pronta para ser utilizada, bastando instanciar um objeto do tipo definido e executar a função parse, passando como parâmetros: a string de input a ser parseada, a instância da gramática previamente definida e a regra de avanço dentro da string que se deseja analisar.

std::string input = "10, 21 369, 642";
RealGrammar grammar;
                                      // ‘space_p’ ignora espaços e tabulações
parse_info<> info = parse(input, grammar, space_p); 

if (info.full()) // Verifica se o input foi totalmente parseado
    std::cout << "Conteúdo parseado com sucesso";

       A função parse retorna, entre outras coisas, o resultado do parseamento através do tipo parse_info<>. Caso o parser tenha totalmente parseado o input fornecido, a função-membro parse_info<>::full() irá retornar verdadeiro.

Boost::function e Gramáticas Com  Ações Semânticas 

       No caso de desejar utilizar ações semânticas em conjunto com funções-membro da classe que define a nossa gramática, podemos recorrer a boost::function. Através da utilização de boost::function podemos fazer com que funções-membro sejam chamadas durante o processo de análise gramatical.
       
       Por exemplo, reproduzindo a gramática anteriormente definida, teríamos:

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

       Os elementos destacados em azul e negrito servem para indicar os pontos de alteração mais relevantes no código.
       
       Começamos definindo uma variável-membro action_ do tipo boost::function com uma assinatura compatível com a da função-membro desejada Action.
       
       A seguir, no construtor da classe RealGrammar, inicializamos action_ para executar a função-membro Action quando disparada por uma ação semântica. Essa associação é feita através de boost::bind, que recebe como parâmetros o endereço da função-membro &RealGrammar::Action e o ponteiro this.
       
       Na definição da regra rule_ associamos a ação semântica a self.action_ que atua como um delegate que executa a função-membro desejada.
       
       Com a atuação conjunta de boost::function e boost::bind é possível obter a funcionalidade desejada, ou seja, utilizar-se funções-membro como parâmetros de ações semânticas.


Sumário

       Este tutorial teve com objetivo introduzir o Spirit framework aos programadores C++ que fazem utilização do Visual C++ como a sua principal ferramenta de desenvolvimento no dia-a-dia.
       
       A escalabilidade e robustez do Spirit permite escrever de parser simples e triviais a parsers com gramáticas extremamente complexas e ambíguas, com uma sintaxe que se aproxima ao máximo de EBNF, sem abandonar os conceitos e a expressividade de C++ .
       
       O Spirit representa, atualmente, em conjunto com a Boost Library, o que existe de mais moderno em técnicas de desenvolvimento em C++, tais como: Meta-Programming, Policy Classes e Expression Templates.
       

Exemplos

       Junto com o artigo você encontrará dois projetos em Visual C++ 2003 e respectivos makefiles (caso deseje trabalhar a partir da linha de comando). Esses projetos fazem referência à variável de ambiente $(BOOST) que deve estar definida no sistema e apontar para o diretório onde se encontra instalada a Boost Library.
       

Sobre os Autores

       Eduardo Bezerra, Engenheiro sênior desenvolvedor de software, especializado em C++, com mais de 19 anos de experiência prática. Atua em projetos nas plataformas .NET / Windows e UNIX. Possui a certificação Microsoft Certified Application Developer (MCAD) Charter Member. É co-autor do livro C++Builder Developer's Guide, SAM'S Publishing Co.
       Gabriel Wernersbach, Analista de Sistemas, especializado em C e C++, com mais de 9 anos de experiência prática. Atua em projetos nas plataformas .NET / Windows e Unix. Possui grande experiência na migração de software entre sistemas operacionais e atualmente é responsável pela manutenção de vários sistemas, incluindo uma interface SQL ANSI feita exclusivamente com Spirit de sua própria autoria.

Sobre o Artigo
       
       Este artigo é propriedade intelectual de Gabriel Wernersbach e Eduardo Bezerra, cedido apenas para o MSDN Brasil (www.microsoft.com/brasil/msdn) e seus usuários. É expressamente proibida a reprodução deste artigo por terceiros, sem autorização formal do autor, como inclusão em livros de outros autores e sites não autorizados.

