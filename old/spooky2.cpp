// #include "../include/spooky2.h"
// #include "../../common/include/int128.h"

// namespace sbl {

// UInt128 SpookyV2::Hash128(IN const Ptr ptr, IN SzInt size)
// {
//     Initialize();
//     Update(ptr, size);
//     return Final();
// }

// UInt64 SpookyV2::Hash64(IN const Ptr ptr, IN SzInt size)
// {
//     return Hash128(ptr, size).high;
// }

// UInt32 SpookyV2::Hash32(IN const Ptr ptr, IN SzInt size)
// {
//     return static_cast<Int32>(Hash128(ptr, size).high);
// }

// SpookyV2::Mixer::Mixer(): state{ 0 } {}

// void SpookyV2::Mixer::Init(IN const SpookyV2& parent)
// {
//     state[0] = state[3] = state[6] = state[9] = parent.state[0];
//     state[1] = state[4] = state[7] = state[10] = parent.state[1];
//     state[2] = state[5] = state[8] = state[11] = parent.CONSTANT;
// }

// void SpookyV2::Mixer::Set(IN const SpookyV2& parent)
// {
//     int index = 0;
//     FAST_LOOP(12, state[index] = parent.state[index]; ++index);
// }

// UInt64& SpookyV2::Mixer::operator[](IN SzInt index)
// {
//     return state[index];
// }

// void SpookyV2::Mixer::Mix(IN const UInt64* data)
// {

//     state[0] += data[0];
//     state[2] ^= state[10];
//     state[11] ^= state[0];
//     state[0] = SHIFT_ROT_L(state[0], 11);
//     state[11] += state[1];
//     state[1] += data[1];
//     state[3] ^= state[11];
//     state[0] ^= state[1];
//     state[1] = SHIFT_ROT_L(state[1], 32);
//     state[0] += state[2];
//     state[2] += data[2];
//     state[4] ^= state[0];
//     state[1] ^= state[2];
//     state[2] = SHIFT_ROT_L(state[2], 43);
//     state[1] += state[3];
//     state[3] += data[3];
//     state[5] ^= state[1];
//     state[2] ^= state[3];
//     state[3] = SHIFT_ROT_L(state[3], 31);
//     state[2] += state[4];
//     state[4] += data[4];
//     state[6] ^= state[2];
//     state[3] ^= state[4];
//     state[4] = SHIFT_ROT_L(state[4], 17);
//     state[3] += state[5];
//     state[5] += data[5];
//     state[7] ^= state[3];
//     state[4] ^= state[5];
//     state[5] = SHIFT_ROT_L(state[5], 28);
//     state[4] += state[6];
//     state[6] += data[6];
//     state[8] ^= state[4];
//     state[5] ^= state[6];
//     state[6] = SHIFT_ROT_L(state[6], 39);
//     state[5] += state[7];
//     state[7] += data[7];
//     state[9] ^= state[5];
//     state[6] ^= state[7];
//     state[7] = SHIFT_ROT_L(state[7], 57);
//     state[6] += state[8];
//     state[8] += data[8];
//     state[10] ^= state[6];
//     state[7] ^= state[8];
//     state[8] = SHIFT_ROT_L(state[8], 55);
//     state[7] += state[9];
//     state[9] += data[9];
//     state[11] ^= state[7];
//     state[8] ^= state[9];
//     state[9] = SHIFT_ROT_L(state[9], 54);
//     state[8] += state[10];
//     state[10] += data[10];
//     state[0] ^= state[8];
//     state[9] ^= state[10];
//     state[10] = SHIFT_ROT_L(state[10], 22);
//     state[9] += state[11];
//     state[11] += data[11];
//     state[1] ^= state[9];
//     state[10] ^= state[11];
//     state[11] = SHIFT_ROT_L(state[11], 46);
//     state[10] += state[0];
// }

// void SpookyV2::Mixer::End(IN const UInt64* data)
// {
//     state[0] += data[0];
//     state[1] += data[1];
//     state[2] += data[2];
//     state[3] += data[3];
//     state[4] += data[4];
//     state[5] += data[5];
//     state[6] += data[6];
//     state[7] += data[7];
//     state[8] += data[8];
//     state[9] += data[9];
//     state[10] += data[10];
//     state[11] += data[11];

//     for(int i = 0; i < 3; ++i) {
//         state[11] += state[1];
//         state[2] ^= state[11];
//         state[1] = SHIFT_ROT_L_UNSIGNED(state[1], 44);
//         state[0] += state[2];
//         state[3] ^= state[0];
//         state[2] = SHIFT_ROT_L_UNSIGNED(state[2], 15);
//         state[1] += state[3];
//         state[4] ^= state[1];
//         state[3] = SHIFT_ROT_L_UNSIGNED(state[3], 34);
//         state[2] += state[4];
//         state[5] ^= state[2];
//         state[4] = SHIFT_ROT_L_UNSIGNED(state[4], 21);
//         state[3] += state[5];
//         state[6] ^= state[3];
//         state[5] = SHIFT_ROT_L_UNSIGNED(state[5], 38);
//         state[4] += state[6];
//         state[7] ^= state[4];
//         state[6] = SHIFT_ROT_L_UNSIGNED(state[6], 33);
//         state[5] += state[7];
//         state[8] ^= state[5];
//         state[7] = SHIFT_ROT_L_UNSIGNED(state[7], 10);
//         state[6] += state[8];
//         state[9] ^= state[6];
//         state[8] = SHIFT_ROT_L_UNSIGNED(state[8], 13);
//         state[7] += state[9];
//         state[10] ^= state[7];
//         state[9] = SHIFT_ROT_L_UNSIGNED(state[9], 38);
//         state[8] += state[10];
//         state[11] ^= state[8];
//         state[10] = SHIFT_ROT_L_UNSIGNED(state[10], 53);
//         state[9] += state[11];
//         state[0] ^= state[9];
//         state[11] = SHIFT_ROT_L_UNSIGNED(state[11], 42);
//         state[10] += state[0];
//         state[1] ^= state[10];
//         state[0] = SHIFT_ROT_L_UNSIGNED(state[0], 54);
//     }
// }

// SpookyV2::SpookyV2(): seed{ CONSTANT, CONSTANT }, length(0), remaind(0), unhashed{ 0 }
// {
//     state[0] = seed.high;
//     state[1] = seed.low;
// }

// SpookyV2::SpookyV2(IN UInt128 seed): seed(seed), length(0), remaind(0), unhashed{ 0 }
// {
//     state[0] = seed.high;
//     state[1] = seed.low;
// }

// void SpookyV2::Initialize()
// {
//     length   = 0;
//     remaind  = 0;
//     state[0] = seed.high;
//     state[1] = seed.low;
// }

// void SpookyV2::Update(const Ptr ptr, SzInt size)
// {
//     SzInt   newLength = remaind + size;
//     SzInt   rest;
//     Ptr     temp;
//     UInt64* end;
//     Mixer   mixer;

//     // Short: temporary storage
//     if(newLength < BUFFER_SIZE) {
//         UByte* t = reinterpret_cast<UByte*>(unhashed) + remaind;
//         std::memcpy(t, ptr, size);
//         length += size;
//         remaind = newLength;
//         return;
//     }

//     // Init
//     if(length < BUFFER_SIZE) {
//         mixer.Init(*this);
//     }
//     else {
//         mixer.Set(*this);
//     }
//     length += size;

//     if(remaind) {
//         SzInt prefix = BUFFER_SIZE - remaind;
//         std::memcpy(reinterpret_cast<UByte*>(unhashed) + remaind, ptr, prefix);

//         temp = unhashed;
//         mixer.Mix(static_cast<UInt64*>(temp));
//         mixer.Mix(static_cast<UInt64*>(temp) + NUM_OF_STATE);

//         temp = static_cast<UByte*>(ptr) + prefix;
//         length -= prefix;
//     }

//     else {
//         temp = ptr;
//     }

//     end  = static_cast<UInt64*>(temp) + (size / BLOCK_SIZE) * NUM_OF_STATE;    // blcok amount * state
//     rest = size - (reinterpret_cast<UByte*>(end) - static_cast<UByte*>(temp)); // remaind

//     while(temp < end) {
//         mixer.Mix(temp);
//         temp = static_cast<UInt64*>(temp) + NUM_OF_STATE;
//     }

//     if(!(static_cast<WordPtr>(temp) & 0x7)) {
//         while(temp < end) {
//             mixer.Mix(temp);
//             temp = static_cast<UInt64*>(temp) + NUM_OF_STATE;
//         }
//     }
//     else {
//         while(temp < end) {
//             memcpy(unhashed, temp, BLOCK_SIZE);
//             mixer.Mix(unhashed);
//             temp = static_cast<UInt64*>(temp) + NUM_OF_STATE;
//         }
//     }

//     // stuff away the last few bytes
//     remaind = rest;
//     memcpy(unhashed, end, rest);

//     int index = 0;
//     FAST_LOOP(12, state[index] = mixer[index]; ++index);
// }

// UInt128 SpookyV2::Final()
// {
//     UInt64* data = reinterpret_cast<UInt64*>(unhashed);
//     SzInt   rest = remaind;

//     Mixer mixer;
//     mixer.Set(*this);

//     // m_data can contain two blocks; handle any whole first block
//     if(remaind >= BLOCK_SIZE) {
//         mixer.Mix(data);
//         data += NUM_OF_STATE;
//         remaind -= BLOCK_SIZE;
//     }

//     // mix in the last partial block, and the length mod sc_blockSize
//     std::memset(reinterpret_cast<UByte*>(data) + remaind, 0, BLOCK_SIZE - remaind);
//     *(reinterpret_cast<UByte*>(data) + BLOCK_SIZE - 1) = static_cast<UByte>(remaind);

//     // do some final mixing
//     mixer.End(data);
//     return UInt128(mixer[0], mixer[1]);
// }

// void SpookyV2::Initialize(IN UInt128 seed)
// {
//     this->seed = seed;
//     Initialize();
// }

// } // namespace sbl