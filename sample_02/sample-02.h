#ifndef SAMPLE_02_H
    #define SAMPLE_02_H

#include "stdafx.h"

using namespace boost::spirit;

namespace sample // Begin namespace sample
{

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
    static const int tag_id       = 1;
    static const int command_id   = 2;
    static const int property_id  = 3;
    static const int str_block_id = 4;

    template <typename ScannerT>
    struct definition
    {
        definition(SimpleGrammar const& /*self*/)
        {
            // Uma TAG pode ser encontrada uma ou mais vezes no texto
            //
            tag_ = +(
                       ch_p('<')  // Inicio da TAG
                    >> command_   // Um comando
                    >> *property_ // Zero ou mais propriedades
                    >> ch_p('>')  // Fim da TAG

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
            property_ = (as_lower_d[str_p("src")] >> ch_p('=') >> leaf_node_d[ str_block_ ])
                        |
                        (as_lower_d[str_p("alt")] >> ch_p('=') >> leaf_node_d[ str_block_ ])
                        ;
            
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

        rule<ScannerT, parser_context<>, parser_tag<tag_id> >       tag_;       
        rule<ScannerT, parser_context<>, parser_tag<command_id> >   command_;   
        rule<ScannerT, parser_context<>, parser_tag<property_id> >  property_;  
        rule<ScannerT, parser_context<>, parser_tag<str_block_id> > str_block_; 

        rule<ScannerT, parser_context<>, parser_tag<tag_id> > const& start() const
        {
            return tag_;
        }
        
        symbols<> reserved_;
    };
};

};//End of namespace sample

#endif

