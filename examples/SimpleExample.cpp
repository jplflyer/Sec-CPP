//
// This is a simple example of using Sec-CPP to generate a KeyPair
// using the keys to generate a signed JWT, and then verifying the
// token.
//

#include <iostream>
#include <chrono>

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

void demonstrateBCrypt() {

}
