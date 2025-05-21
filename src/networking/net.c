#include "../include/net.h"

static func_ptr_t *sendFuncList;
static func_ptr_b *buildFuncList;
static int func_index = 0;

uint16_t flip_short(uint16_t short_int)
{
    uint32_t first_byte = *((uint8_t *)(&short_int));
    uint32_t second_byte = *((uint8_t *)(&short_int) + 1);
    return (first_byte << 8) | (second_byte);
}

uint32_t flip_long(uint32_t long_int)
{
    uint32_t first_byte = *((uint8_t *)(&long_int));
    uint32_t second_byte = *((uint8_t *)(&long_int) + 1);
    uint32_t third_byte = *((uint8_t *)(&long_int) + 2);
    uint32_t fourth_byte = *((uint8_t *)(&long_int) + 3);
    return (first_byte << 24) | (second_byte << 16) | (third_byte << 8) | (fourth_byte);
}

/*
 * Flip two parts within a byte
 * For example, 0b11110000 will be 0b00001111 instead
 * This is necessary because endiness is also relevant to byte, where there are two fields in one byte.
 * number_bits: number of bits of the less significant field
 * */
uint8_t flip_byte(uint8_t byte, int num_bits)
{
    uint8_t t = byte << (8 - num_bits);
    return t | (byte >> num_bits);
}

uint16_t htons(uint16_t hostshort)
{
    return flip_short(hostshort);
}

void register_func(func_ptr_b buildFunc, func_ptr_t sendFunc)
{
    if (sendFuncList == NULL)
    {
        sendFuncList = kmalloc(countNetworkDevices() * sizeof(func_ptr_t));
    }
    if (buildFuncList == NULL)
    {
        buildFuncList = kmalloc(countNetworkDevices() * sizeof(func_ptr_b));
    }
    if (func_index >= countNetworkDevices())
    {
        return;
    }
    sendFuncList[func_index] = sendFunc;
    buildFuncList[func_index] = buildFunc;
    func_index += 1;
    return;
}

bool send_package(uint8_t *dst, void *data, int len, uint16_t type)
{
    bool is_success = false;
    int index = 0;
    do
    {
        void *package = buildFuncList[index](dst, data, len, type);
        is_success = sendFuncList[index++](package, len);
        package = NULL;
    } while (!is_success && index < countNetworkDevices());
    return is_success;
}
