//
//  RSA.cpp
//  RSA
//
//  Created by ivan sarno on 21/08/15.
//  Copyright (c) 2015 ivan sarno. All rights reserved.
//
//Version V.3.4

#include "RSA.h"

using namespace RSA;

BigInteger RSA::Encrypt(const BigInteger &message, const BigInteger &pubkey, const BigInteger &modulus, unsigned int size)
{
    if(modulus > 1 && pubkey > 1 && size >= 64)
    {
        BigInteger result = Utils::mod_pow(message, pubkey, modulus);
        return result;
    }
    return 0;
}

BigInteger RSA::Decrypt(const BigInteger &message, const BigInteger &privkey, const BigInteger &modulus, unsigned int size)
{
    if(modulus > 1 && privkey > 1 && size >= 64)
    {
        BigInteger result = Utils::mod_pow(message, privkey, modulus);
        return result;
    }
    return 0;
}


//check the compliance with security standard
bool E_check(const BigInteger &E, const BigInteger &Phi)
{
    return coprime(E,Phi) && (E-1!=(Phi>>2)) && (E-1!=(Phi>>1)) && E > 1;
}

//check the compliance with security standard
bool Q_check(BigInteger Q, BigInteger P, unsigned long distance)
{
    BigInteger dif = (P-Q);
    P=(P-1)>>1;
    Q=(Q-1)>>1;

    return coprime(P,Q) && (abs(dif) > distance);
}

bool RSA::Keygen(BigInteger &pubkey, BigInteger &privkey, BigInteger &modulus, RSA::Utils::Generator gen, unsigned int size, unsigned int precision, unsigned long distance)
{
    if(size < 64 || precision < 2)
        return false;



    BigInteger primeP= Prime::Generates(gen, size/2); //generates prime number for key mod
    BigInteger primeQ= Prime::Generates(gen, size/2);

#ifdef RSA_QCHECK
    while (!Q_check(primeQ,primeP, distance)) //make sure it is appropriate for security standards
    {
        primeQ= Prime::Generates(gen, size/2);
    }
#endif

    BigInteger Phi = (primeP-1) * (primeQ-1);
    modulus = primeP * primeQ; //Mod of key

    pubkey = gen.get(size);
    pubkey = pubkey % modulus;//public key


    while (!E_check(pubkey, Phi)) //make sure it is appropriate for security standards
    {
        pubkey++;
    }

    privkey = Utils::inverse(pubkey, Phi, size); //private key

    return true;
}
