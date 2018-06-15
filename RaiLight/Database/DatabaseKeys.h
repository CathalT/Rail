#pragma once

namespace rail::key
{
    namespace bytes
    {
        constexpr char* const SEED = "seed";
        constexpr char* const PASS_HASH = "pass_hash";
        constexpr char* const PASS_SALT = "pass_salt";
        constexpr char* const KEY_SALT = "key_salt";
        constexpr char* const LATEST_BLOCK = "latest_block_";
        constexpr char* const SEED_IV = "seed_iv";
    }

    namespace uint
    {
        constexpr char* const ACCOUNT_INDEX = "acc_idx";
        constexpr char* const BALANCE = "balance_";
        constexpr char* const PENDING_BALANCE = "pend_bal_";
    }
    
    namespace bools
    {
        constexpr char* const ACCOUNT_OPEN = "acc_open_";
    }
}