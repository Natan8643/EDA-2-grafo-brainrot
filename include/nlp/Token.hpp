/*
    esse arquivo define o struct do token (token = palavra qualquer em uma fras) que sera utilizado no SpacyProcessor

    o lemma é a forma canonica da palavra ex: caras
                                               cara


*/

#pragma once

#include <string>

struct Token
{
    std::string original;
    std::string lemma;
};