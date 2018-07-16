#pragma once

namespace TestUtils
{
    constexpr char TEST_PASSWORD[] = "test123";
    constexpr char TEST_ADDRESS_ONE[] = "xrb_1oocugiybqxypp7f9qpt4d48dxbqtczhg68z5db17xr3sbosdq7bq8zufm9m";
    constexpr char TEST_SEED_ONE[] = "FBE0A52D1A5A4FF9E2F0B5F716BE07FAEF5E21797FD60A2E3858001A3FCD54A3";

    constexpr char TEST_ADDRESS_TWO[] = "xrb_1sjhm4guw13t6d5nynwqrd34eh7kwiatrh7jrihcsqizd683ghepxnomcemt";
    constexpr char TEST_SEED_TWO[] = "34E41BE2C40454F4E853C6633243689068F95BDB03286A4283CC65C9C73A6F67";

    constexpr std::array<std::byte, 32> TEST_SEED_ONE_BYTES = 
    { std::byte(0xFB), std::byte(0xE0), std::byte(0xA5), std::byte(0x2D), std::byte(0x1A), std::byte(0x5A), std::byte(0x4F), std::byte(0xF9),
      std::byte(0xE2), std::byte(0xF0), std::byte(0xB5), std::byte(0xF7), std::byte(0x16), std::byte(0xBE), std::byte(0x07), std::byte(0xFA),
      std::byte(0xEF), std::byte(0x5E), std::byte(0x21), std::byte(0x79), std::byte(0x7F), std::byte(0xD6), std::byte(0x0A), std::byte(0x2E),
      std::byte(0x38), std::byte(0x58), std::byte(0x00), std::byte(0x1A), std::byte(0x3F), std::byte(0xCD), std::byte(0x54), std::byte(0xA3) };
    
    constexpr std::array<std::byte, 32> TEST_SEED_TWO_BYTES = 
    { std::byte(0x34), std::byte(0xE4), std::byte(0x1B), std::byte(0xE2), std::byte(0xC4), std::byte(0x04), std::byte(0x54), std::byte(0xF4),
      std::byte(0xE8), std::byte(0x53), std::byte(0xC6), std::byte(0x63), std::byte(0x32), std::byte(0x43), std::byte(0x68), std::byte(0x90),
      std::byte(0x68), std::byte(0xF9), std::byte(0x5B), std::byte(0xDB), std::byte(0x03), std::byte(0x28), std::byte(0x6A), std::byte(0x42),
      std::byte(0x83), std::byte(0xCC), std::byte(0x65), std::byte(0xC9), std::byte(0xC7), std::byte(0x3A), std::byte(0x6F), std::byte(0x67) };

    constexpr std::array<std::byte, 32> TEST_PASS_KEY_SALT =
    { std::byte(0x8b), std::byte(0x46), std::byte(0xe4), std::byte(0xed), std::byte(0x51), std::byte(0xbd), std::byte(0x5b), std::byte(0x24), 
      std::byte(0xd0), std::byte(0x15), std::byte(0xd7), std::byte(0xf9), std::byte(0xf2), std::byte(0x2a), std::byte(0xb7), std::byte(0xc3),
      std::byte(0xa9), std::byte(0xe7), std::byte(0xa6), std::byte(0x34), std::byte(0xdc), std::byte(0x46), std::byte(0xe3), std::byte(0x3b),
      std::byte(0x08), std::byte(0x3f), std::byte(0xd2), std::byte(0xf9), std::byte(0x7d), std::byte(0x83), std::byte(0xc4), std::byte(0xb0) };

    constexpr std::array<std::byte, 16> TEST_SEED_ONE_IV =
    { std::byte(0x02), std::byte(0xb0), std::byte(0x5f), std::byte(0x01), std::byte(0xa9), std::byte(0x77), std::byte(0x82), std::byte(0xe5),
      std::byte(0x6c), std::byte(0x97), std::byte(0xf0), std::byte(0x2e), std::byte(0x60), std::byte(0x87), std::byte(0xcd), std::byte(0x34) };

    const std::vector<std::byte> ENCRYPTED_SEED_ONE =
    { std::byte(0x31), std::byte(0xfb), std::byte(0xba), std::byte(0x8d), std::byte(0x9c), std::byte(0xb1), std::byte(0x7c), std::byte(0x89),
      std::byte(0x8a), std::byte(0x6c), std::byte(0x73), std::byte(0xec), std::byte(0x5e), std::byte(0x6c), std::byte(0xb8), std::byte(0xde),
      std::byte(0xdd), std::byte(0x89), std::byte(0x22), std::byte(0xe2), std::byte(0xd2), std::byte(0x35), std::byte(0x78), std::byte(0xdb),
      std::byte(0xce), std::byte(0x02), std::byte(0x71), std::byte(0x41), std::byte(0xe5), std::byte(0xd1), std::byte(0x8d), std::byte(0x19),
      std::byte(0x91), std::byte(0x6a), std::byte(0x2f), std::byte(0x90), std::byte(0x66), std::byte(0x82), std::byte(0x1d), std::byte(0xda),
      std::byte(0x94), std::byte(0x77), std::byte(0xe0), std::byte(0x6c), std::byte(0x90), std::byte(0xcc), std::byte(0xdb), std::byte(0x77) };

}