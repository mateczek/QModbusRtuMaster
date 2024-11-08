#include "reinterpretervariable.h"

reinterpreterVariable::reinterpreterVariable(QObject *parent)
    : QObject{parent}
{}

std::tuple<int16_t, int16_t, uint16_t, uint16_t> reinterpreterVariable::get16bitValue(uint16_t zm)
{
    union{
        uint16_t zmu;
        int16_t zmi;
        uint8_t tab[2];
    }unia;
    unia.zmu=zm;
    int16_t temp0=unia.zmi;
    uint16_t temp1=unia.zmu;
    std::swap(unia.tab[0],unia.tab[1]);
    int16_t temp2=unia.zmi;
    uint16_t temp3=unia.zmu;
    return std::make_tuple(temp0,temp2,temp1,temp3);
}

std::tuple<int32_t, uint32_t, float, int32_t, uint32_t, float,int32_t, uint32_t,float, int32_t, uint32_t, float> reinterpreterVariable::get32bitValue(uint32_t zm)
{
    union{
        uint32_t zm_uint;
        int32_t  zm_int;
        float zm_float;
        uint8_t tabByte[4];
        uint16_t tabWord[2];
    }unia;
    unia.zm_uint=zm;

    int32_t zm_int_ABCD = unia.zm_int;
    uint32_t zm_uint_ABCD = unia.zm_uint;
    float zm_float_ABCD=unia.zm_float;
    std::swap(unia.tabWord[0],unia.tabWord[1]);
    int32_t zm_int_CDAB = unia.zm_int;
    uint32_t zm_uint_CDAB = unia.zm_uint;
    float zm_float_CDAB=unia.zm_float;
    std::swap(unia.tabByte[0],unia.tabByte[1]);
    std::swap(unia.tabByte[2],unia.tabByte[3]);
    int32_t zm_int_DCBA = unia.zm_int;
    uint32_t zm_uint_DCBA = unia.zm_uint;
    float zm_float_DCBA=unia.zm_float;
    std::swap(unia.tabWord[0],unia.tabWord[1]);
    int32_t zm_int_BADC = unia.zm_int;
    uint32_t zm_uint_BADC = unia.zm_uint;
    float zm_float_BADC=unia.zm_float;
    return std::make_tuple(zm_int_ABCD,zm_uint_ABCD,zm_float_ABCD,zm_int_CDAB,zm_uint_CDAB,zm_float_CDAB,zm_int_DCBA,zm_uint_DCBA,zm_float_DCBA,zm_int_BADC,zm_uint_BADC,zm_float_BADC);
}

std::tuple<int64_t, uint64_t, double, int64_t, uint64_t, double> reinterpreterVariable::get64bitValue(uint64_t zm)
{
    union{
        uint64_t zm_uint;
        int64_t  zm_int;
        double zm_double;
        uint8_t tabByte[8];
    }unia;
    unia.zm_uint=zm;
    int64_t zm_intABCDEFGH=unia.zm_int;
    uint64_t zm_uintABCDEFGH=unia.zm_uint;
    double zm_doubleABCDEFGH=unia.zm_double;
    for(int i=0;i<4;i++){
        std::swap(unia.tabByte[i],unia.tabByte[7-i]);
    }
    int64_t zm_intHGFEDCBA=unia.zm_int;
    uint64_t zm_uintHGFEDCBA=unia.zm_uint;
    double zm_doubleHGFEDCBA=unia.zm_double;
    return std::make_tuple(zm_intABCDEFGH,zm_uintABCDEFGH,zm_doubleABCDEFGH,zm_intHGFEDCBA,zm_uintHGFEDCBA,zm_doubleHGFEDCBA);
}
