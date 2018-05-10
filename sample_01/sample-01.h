#ifndef SAMPLE_01_H
    #define SAMPLE_01_H

#include "stdafx.h"

using namespace boost;
using namespace boost::spirit;

namespace sample // Begin namespace sample
{

//    Print
//
//    :: As ações semânticas somente imprimem na
//       tela o que foi encontrado pela regra ::
//
struct Print
{
    static void String(char const* Str, char const* End);
    static void Char(char const C);

private:
    static bool isProperty_;
};

//    SkipGrammar
//
//    :: Regra para avanço dentro do texto ser analizado ::
//
struct SkipGrammar : public grammar<SkipGrammar>
{
    template <typename ScannerT>
    struct definition
    {
        definition(SkipGrammar const& /*self*/)
        {
            skip =
                // Ignora espacos em branco
                //
                space_p
                |
                // Ignora linhas que comecem com '//'
                //
                // Essa regra equivale a comment_p("//");
                //
                lexeme_d
                [
                    ch_p('/') >> ch_p('/') >> *(anychar_p -(!ch_p('\r') >> ch_p('\n')))
                ]
                |
                // Ignora a partir de um '/*' até um '*/'
                //
                // Essa regra equivale a comment_p("/*", "*/")
                //
                lexeme_d
                [
                    ch_p('/') >> ch_p('*')
                    >> *((!ch_p('\r') >> ch_p('\n') | anychar_p) - (ch_p('*') >> ch_p('/'))) >>
                    ch_p('*') >> ch_p('/')
                ];
        }

        rule<ScannerT> skip;

        rule<ScannerT> const& start() const
        {
            return skip;
        }
    };
};


//    SimpleGrammar
//
//    :: Implementa uma gramatica simples ::
//
struct SimpleGrammar : public grammar<SimpleGrammar>
{
    // Ctor
    SimpleGrammar()
    {
        action_ = bind(&SimpleGrammar::Action, this, _1, _2);
    }

    template <typename ScannerT>
    struct definition
    {
        definition(SimpleGrammar const& self)
        {
            // Uma TAG pode ser encontrada uma ou mais vezes no texto
            //
            tag_ = +(
                       ch_p('<')  [&Print::Char]   // Inicio da TAG
                                  // Demonstra o uso de 'boost::function'
                    >> command_   [self.action_]   // Um comando
                    >> *property_                  // Zero ou mais propriedades
                    >> ch_p('>')  [&Print::Char]   // Fim da TAG

                    );
            
            // Apenas checa se a palavra faz parte do grupo de comandos válidos
            //
            command_ = reserved_;
            
            // Essa regra consome strings no formato:
            //
            // src = "some text" ou alt = "some text"
            //
            // Sendo que a palavra que designa a propriedade é convertida para minusculas
            // antes da comparacao. Dessa forma a regra suporta qualquer combinacao de
            // minusculas + maiusculas
            //
            property_ = (
                           as_lower_d[str_p("src")] [&Print::String]
                        >> ch_p('=')                [&Print::Char]
                        >> str_block_               [&Print::String]

                        )
                        |
                        (
                           as_lower_d[str_p("alt")] [&Print::String]

                        >> ch_p('=')                [&Print::Char]
                        >> str_block_               [&Print::String]

                        );
            
            // Pega qualquer grupo de caracteres dentro de aspas
            //
            str_block_ = confix_p('"', *anychar_p, '"');
            
            // Comandos validos
            reserved_ = "ADDRESS", "BLOCKQUOTE", "IMG", "HREF";
            
            // Habilita o modo debug para cada regra caso estejamos com debug definido
    
            BOOST_SPIRIT_DEBUG_RULE(tag_);
            BOOST_SPIRIT_DEBUG_RULE(command_);
            BOOST_SPIRIT_DEBUG_RULE(property_);
            BOOST_SPIRIT_DEBUG_RULE(str_block_);
        }

        rule<ScannerT> tag_       ,
                       command_   ,
                       property_  ,
                       str_block_ ;

        rule<ScannerT> const& start() const
        {
            return tag_;
        }
        
        symbols<> reserved_;
    };

private:
    // Member functions
    void Action(const char* First, const char* Last) const
    {
        std::cout << std::string(First, Last);
    }

    function<void (const char*, const char*)> action_;
};

};//End of namespace sample

#endif

