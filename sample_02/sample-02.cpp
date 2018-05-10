//
//  Copyright (c) 2004-2005 Gabriel W Farinas, Eduardo Bezerra
//
//  Use, modification and distribution is free as long as you give credit to 
//  the author. This grammar is provided with no warranty of any type or kind.
//  
//  Usar, modificar e distribuir é gratuito conquanto você dê credito ao autor.
//  Essa gramatica é disponibilizada sem qualquer tipo de garantia.
//
//  Exemplo 2
//  Parte do artigo "Spirit Tutorial"
//
//  Analise gramatical, feita a partir do conteúdo de um arquivo
//  Utiliza Abstract Syntax Tree (AST)
//

#include "stdafx.h"

#include "sample-02.h"

using std::back_inserter;
using std::cerr;
using std::copy;
using std::cout;
using std::endl;
using std::ifstream;
using std::ios;
using std::istream_iterator;
using std::map;
using std::string;

using namespace sample;


//
//    _tmain
//
int _tmain(int argc, _TCHAR* argv[])
{
    cout << "Exemplo de gramatica usando Spirit e Abstract Syntax Tree (AST)" << endl;

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

    // Gera uma arvore AST
    tree_parse_info<> info = ast_parse(s.c_str(), grammar, skipper);

    if (info.full) // O input foi todo consumido ?
    {
        map<parser_id, string> rules;

        // Troca numeros por strings para melhor compreensão das tags xml
        rules[ SimpleGrammar::tag_id       ] = "tag";
        rules[ SimpleGrammar::command_id   ] = "command";
        rules[ SimpleGrammar::property_id  ] = "property";
        rules[ SimpleGrammar::str_block_id ] = "string";
        
        // Processa e imprime um xml com o conteudo da AST gerada
        tree_to_xml(cout, info.trees, s, rules);
    }
    else
    {
        // Exibe o ponto em que a análise foi interrompida
        cout << const_cast<char*>(info.stop) << endl;
        return -1;
    }
}

