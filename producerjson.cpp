#include <eosiolib/eosio.hpp>
#include <eosiolib/asset.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosio.system/eosio.system.hpp>

using eosio::asset;
using std::string;

class producerjson_contract : public eosio::contract {
 public:
  producerjson_contract(account_name self) :
    eosio::contract(self),
    producerjson(_self, _self) {}


  // A simple store for a producer's json.
  void set(const account_name owner, const string json) {
    require_auth(owner);

    // Check producer info table. Owner should exist (has called regproducer) and be activated.
    typedef eosio::multi_index<N(producers), eosiosystem::producer_info> producer_info_t;
    producer_info_t _producers(N(eosio), N(eosio));
    auto prod = _producers.get(owner,    "user is not a producer");
    eosio_assert(prod.is_active == true, "user is not an active producer");
    eosio_assert(prod.total_votes > 0.0, "user is not an active producer");

    // Quick check to remind the user the payload must be json.
    eosio_assert(json[0] == '{',             "payload must be json");
    eosio_assert(json[json.size()-1] == '}', "payload must be json");

    // If entry exists, update it.
    auto target_itr = producerjson.find(owner);
    if (target_itr != producerjson.end()) {
      producerjson.modify(target_itr, owner, [&](auto& j) {
        j.owner = owner;
        j.json  = json;
      });
    } else {  // Otherwise, create a new entry for them.
      producerjson.emplace(owner, [&](auto& j) {
        j.owner = owner;
        j.json  = json;
      });
    }
  }


  // Allows a producer to delete their entry.
  void del(const account_name owner) {
    require_auth(owner);
    auto target_itr = producerjson.find(owner);
    producerjson.erase(target_itr);
  }


 private:
  // @abi table producerjson i64
  struct producerjson {
    account_name owner;
    string       json;

    auto primary_key() const {  return owner;  }
    EOSLIB_SERIALIZE(producerjson, (owner)(json))
  };
  typedef eosio::multi_index<N(producerjson), producerjson> producerjson_table;
  producerjson_table producerjson;
};

EOSIO_ABI(producerjson_contract, (set)(del))
