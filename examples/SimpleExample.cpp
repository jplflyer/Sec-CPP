//
// This is a simple example of using Sec-CPP to generate a KeyPair
// using the keys to generate a signed JWT, and then verifying the
// token.
//

#include <iostream>
#include <chrono>

#include <bcrypt/BCrypt.hpp>

// CommonUsing just does a few "using std::cout" statements. It's here for
// convenience only.
#include <Sec-CPP/CommonUsing.h>

#include <Sec-CPP/KeyPair.h>
#include <Sec-CPP/Sec_CPP.h>

// These are important. We're using nlohmann's JSON. You have other
// choices. But for this example, you need all three includes.
// And you need this #define so it does NOT use pico JSON.
#define JWT_DISABLE_PICOJSON
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/defaults.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>

using JSON = nlohmann::json;

// This is just here so it's easy to print a date.
#include <date/date.h>
using date::operator<<;

static void demonstrateJWT();
static void demonstrateBCrypt();

int main(int, char **) {
    cout << std::boolalpha;
    demonstrateJWT();
    demonstrateBCrypt();
}

void demonstrateJWT() {
    //
    // First, it's absolutely trivial to create new public/private key pair.
    //
    Sec_CPP::KeyPair keys = Sec_CPP::generateKeys();

    cout << "Public Key : " << endl << keys.getPublicKey() << endl << endl
         << "Private Key: " << endl << keys.getPrivateKey() << endl;

    //
    // Now let's create a JWT. You don't need Sec_CPP for that. Everything
    // is in https://github.com/Thalhammer/jwt-cpp. But you do need public
    // and private keys that are formed properly, and that part can be tricky.
    //
    // A JWT can contain arbitrary information, whatever is useful. The token
    // has a header, which is well-defined, and a payload. The payload contains
    // what JWT calls "claims", and it's just JSON. There are 7 well-defined
    // claims, but you can arbitrarily add whatever else you might want,
    // and you don't necessarily need to include the 7 standard claims.
    //
    // We added a custom claim using .set_payload_claim.
    //
    JSON randomJSON = JSON::object();
    randomJSON["hello"] = "Why, hello there.";
    randomJSON["goodbye"] = "See ya later.";

    auto token = jwt::create()
            .set_issuer("Me")           // This can be anything you want.
            .set_type("JWS")
            .set_issued_at( std::chrono::system_clock::now() )
            .set_expires_at( std::chrono::system_clock::now() + 60min)
            .set_payload_claim("foo", jwt::claim( string{"bar"} ))
            .set_payload_claim("some_json", randomJSON)
            .sign(jwt::algorithm::rs512{"", keys.getPrivateKey(), "", ""})
            ;

    // The result of that is just a string.
    cout << endl << "The JWT Token: " << token << endl;

    //
    // You can now pass that around. It has been signed with your private key and
    // can be reviewed with no keys at all, but you will need the public key to
    // verify the signer. In this code, I am using get_payload_json(), which returns
    // the payload as JSON. You can use get_payload(), and it returns a string.
    //
    auto decoded = jwt::decode(token);
    std::chrono::system_clock::time_point issuedAt = decoded.get_issued_at();
    cout << "Issuer: " << decoded.get_issuer() << endl;
    cout << "Issued at: " << issuedAt << endl;

    JSON payload = decoded.get_payload_json();
    cout << "Payload: " << payload.dump(2) << endl;

    //
    // Now let's verify the signature.
    //
    try {
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs512{ keys.getPublicKey(), "", "", "" })
            .with_issuer(decoded.get_issuer())
            ;
        verifier.verify(decoded);
    }
    catch (std::exception &e) {
        cout << "We didn't expect this exception.\n";
    }

    //
    // Let's show what happens if it's not signed properly. For that, we need
    // a different set of keys, and we'll pretend we think it was signed with
    // these keys instead.
    //
    // This time, we'll get an exception.
    //
    try {
        Sec_CPP::KeyPair falseKeys = Sec_CPP::generateKeys();
        auto badVerifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs512{ falseKeys.getPublicKey(), "", "", "" })
            .with_issuer(decoded.get_issuer())
            ;

        badVerifier.verify(decoded);
    }
    catch (std::exception &e) {
        cout << "False verification of the signature threw an exception as expected.\n";
    }

    //
    // If the token passes signature validation with the proper public key, then
    // you know the token was signed with the private key. The most common usage for
    // JWT is a server generates the token and clients then just pass it around but
    // don't have to look at it. The server still has both the public and private keys,
    // and if it passes signature validation, he knows it's not spoofed. He can safely
    // trust the contents. And as you can put anything in the payload you want, you can
    // for instance have things like a username in there. (NOT THE PASSWORD!) You can
    // also include all privileges this user yas, if you're willing to let them be seen
    // by anyone who intercepts the token.
    //
    // If you want anyone to be able to validate the signature, you have to share the
    // public key, but keep the private key secure.
    //
    // Note that you can only trust any of the claims if you trust the signature. It's
    // insufficient to query the issuer field. You MUST validate the signature against
    // an expected key pair.
    //
    // If you were using a big public site (maybe a single signon system), the site
    // would publish the public keys, and you could use those. So I'm going to show how
    // to do that.
    //

    //
    // They keys are just strings, and you don't need the private key to do validation.
    // If you look at KeyPair, you'll see it's just a couple of strings, but let's drive
    // that point home this way.
    //
    // You can get this string however it's convenient. Maybe a REST call. Maybe read it
    // from a file. See KeyPair::writeToFiles() and KeyPair::readFromFiles(), for instance.
    // Here we'll just grab it from the original keypair to point out it's just a string.
    //
    string publicKey = keys.getPublicKey();
    try {
        // This is cut & paste from previous.
        auto verifier = jwt::verify()
            .allow_algorithm(jwt::algorithm::rs512{ publicKey, "", "", "" })
            .with_issuer(decoded.get_issuer())
            ;
        verifier.verify(decoded);
    }
    catch (std::exception &e) {
        cout << "We didn't expect this exception.\n";
    }

    // And there you have it.
}

/**
 * This demonstrates use of bcrypt. Note that we don't provide any special code, as
 * bcrypt is exceedingly easy to use. Let's talk about passwords.
 *
 * SAVING PASSWORDS
 *
 * Passwords should never be stored in the clear. Instead, they should be stored
 * in a one-way hash. You then don't verify the PW entered during login is the
 * same as the one you stored. Instead, you verify that when you use the same hash
 * algorithm, you get the same hash value. There is an exceedingly small chance that
 * two different strings will generate the same hash, but that's deemed to be such a
 * small chance as to be zero.
 *
 * This is still not entirely sufficient. You have to assume a dedicated hacker can
 * download your password table. And there are lookup tables that hackers publish for
 * each other that give the hash values and the password that generated it, so for a lot
 * of passwords people use, it's a trivial lookup. So instead, you also use something called
 * a salt. A salt is randomly generated, and added to the password before hashing. The
 * output includes the salt used, which sounds insecure, but because they're so random,
 * it defeats those lookup tables just mentioned.
 *
 * With BCrypt.hpp, we don't have to worry to much about that. So here's some
 * sample code. Note that we have already done #include <bcrypt/BCrypt.hpp>
 *
 * SAFE PASSWORDS
 *
 * But before the code, let's also talk about quality passwords. Nothing I'm saying
 * should be new, but maybe it is to some readers.
 *
 * Modern password practices are:
 *
 * 1. Use a password tool like NordPass, Dashlane, 1Password, LastPass, et cetera.
 * 2. Let the tool generate long, random passwords. More on this below.
 * 3. What is most important is length, not crazy characters.
 *
 * XKCD years ago pointed out that a password like "donkey ears laid low" is better
 * than "h3ll0". First off, hackers know all the tricks about using numbers instead
 * of letters. The idea is to have the greatest number of unlikely bits. You're likely
 * to remember a sequence of words, but it's much harder to remember a single word with
 * some strange characters tossed in. But ultimately, it's about length. The most number
 * of bits.
 *
 * PASSWORD MANAGERS
 *
 * With modern password tools, you don't even need to remember your passwords. I remember
 * my password manager's passwords (I use more than one), and I remember my login password
 * and a few others. But I have a unique password for every site I use, and I don't have a
 * clue what any of them are. The password manager handles it for me.
 *
 * ABOUT WORKLOAD
 *
 * BCrypt has been out since the late 90s. Moore's law was well understood, so the algorithm
 * included a way of making passwords increasingly difficult to crack as computers grew
 * more capable. This is built into the workload. A workload of 4 means 2^4 cycles, so each
 * time you increase the workload number by 1, you double the time it takes to hash and
 * the time to validate a password.
 *
 * So periodically, as the machines we're using get faster, we need to increase the workload.
 * The current default with libbcrypt is 12. I'm using 15 in this example. The idea is that
 * if you have a lengthy, entirely random password of letters, numbers, digits, and symbols,
 * that it would take years to crack it via brute force using existing equipment. (The theory
 * is quantum computers are totally going to screw with this, of course.)
 *
 * This number may be too high for your usage. On the other hand, it is probably a little
 * low if someone gets your password table and starts trying to crack it on a serious
 * machine.
 *
 * MULTI-FACTOR AUTHENTICATION
 *
 * Sometimes called 2-factor authentication, 2FA, or MFA, this is an important concept. There
 * is better reading out there. I'm not going to demonstrate 2FA or MFA. MFA means "2 or more".
 * The idea is that authentication is based on:
 *
 * 1. Something you know (your password)
 * 2. Something you have (any sort of authentication app or hardware)
 * 3. Something you are (fingerprint, iris scan, et cetera)
 *
 * If your site is critical, you will want to use MFA. You'll need to do more reading
 * elsewhere to find a solution for your needs. I'm not going to demonstrate it here,
 * but I thought it was worth mentioning.
 *
 * Accepted policy is that 1FA (username and password) is no longer sufficient for access
 * to corporate resources. Note that many password management tools have 2FA capabilities
 * built in, and you can get tools like Google Authenticator on your phone.
 *
 * THE CODE
 *
 * But let's get into the code. I'm using a C++ wrapper as part of libbcrypt.
 * It handles generating a salt. So the code is very simple.
 */
void demonstrateBCrypt() {
    cout << "\nDemonstrating bcrypt:\n";
    string originalPW = "this is a random password";
    string testPw = "this is a a different password";
    int bcryptWorkload = 15;

    string hashed = BCrypt::generateHash(originalPW, bcryptWorkload);
    cout << "The password hashed to: " << hashed << endl;

    bool doNotMatch = BCrypt::validatePassword(testPw, hashed);
    bool pleaseMatch = BCrypt::validatePassword(originalPW, hashed);

    cout << "Verify the wrong pw: " << doNotMatch
         << ". And the right one: " << pleaseMatch << endl;

    // The above code takes time to run. That's intentional to avoid
    // brute force attacks. Change bcryptWorkload to 4 and it will run
    // a whole lot faster. But that's not what you want.
    //
    // You can semi-safely store the hashed value in your database and
    // then forget the original password. Just use the validate code
    // when you want to verify a password.
}
