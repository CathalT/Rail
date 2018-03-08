#include "Model\Account.h"

#include "Model\BlockStore.h"

namespace rail
{
    Account::~Account() = default;
    Account::Account(const ByteArray32& _privateKey, const ByteArray32& _publicKey, uint32_t _index, std::string _accountId) :
        privateKey(_privateKey),
        publicKey(_publicKey),
        index(_index),
        accountId(_accountId)
    {}
}