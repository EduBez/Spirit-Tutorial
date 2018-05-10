//
//  Copyright (c) 2004-2005 Gabriel W Farinas, Eduardo Bezerra
//
//  Use, modification and distribution is free as long as you give credit to 
//  the author. This grammar is provided with no warranty of any type or kind.
//  
//  Usar, modificar e distribuir é gratuito conquanto você dê credito ao autor.
//  Essa gramatica é disponibilizada sem qualquer tipo de garantia.
//
//  Exemplo 1
//  Parte do artigo "Spirit Tutorial"
//
//  Analise gramatical, feita a partir do conteúdo de um arquivo
//  Utiliza Semantic Actions
//

#include "stdafx.h"

#include "sample-01.h"

using std::back_inserter;
using std::cerr;
using std::copy;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::istream_iterator;
using std::istream_iterator;
using std::string;

using namespace sample;

bool Print::isProperty_ = false;

//
//    Print
//
void Print::String(char const* Str, char const* End)
{
    string s(Str, End);
    
    if (isProperty_)
    {
        cout << '\t'; // Imprime um tab caso estejamos dentro de uma propriedade
        isProperty_ = false;
    }
    cout << "\t(" << s << ")\n";
}

void Print::Char(char const C)
{
    if (C == '=')
    {
        cout << '\t';
        isProperty_ = true; // Estamos lendo uma propriedade
    }
    cout << '(' << C << ")\n";
}


//
//    _tmain
//
int _tmain(int argc, _TCHAR* argv[])
{
    cout << "Exemplo de gramatica usando Spirit e Acoes Semanticas" << endl;

    if (argc < 2)
    {
        cerr << "Nenhum arquivo foi informado" << endl;
        return 0;
    }

    ifstream in(argv[1]);

    if (!in)
    {
        cerr << "Nao foi possivel abrir o arquivo: " << argv[1] << endl;
        return 0;
    }

    in.unsetf(ios::skipws); // Evita que o stream pule espaços em branco
    string s;

    // Copia para o conteudo do arquivo para a 's'
    copy(istream_iterator<char>(in), istream_iterator<char>(), back_inserter(s));

    SimpleGrammar grammar;
    SkipGrammar skipper;

    parse_info<> info = parse(s.c_str(), grammar, skipper);

    if (info.full) // O input foi todo consumido ?
        return 1;
    else
    {
        // Exibe o ponto em que a análise foi interrompida
        cout << const_cast<char*>(info.stop) << endl;
        return -1;
    }
}

