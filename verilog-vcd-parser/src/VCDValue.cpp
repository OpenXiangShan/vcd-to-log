
#include "VCDValue.hpp"


/*!
*/
VCDValue::VCDValue    (
    VCDBit  value
){
    this -> type = VCD_SCALAR;
    this -> value.val_bit = value;
}

/*!
*/
VCDValue::VCDValue    (
    VCDBitVector *  value
){
    this -> type = VCD_VECTOR;
    this -> value.val_vector= value;
}

/*!
*/
VCDValue::VCDValue    (
    VCDReal value
){
    this -> type = VCD_REAL;
    this -> value.val_real = value;
}


VCDValueType   VCDValue::get_type(){
    return this -> type;
}


/*!
*/
VCDBit       VCDValue::get_value_bit(){
    return this -> value.val_bit;
}


/*!
*/
VCDBitVector * VCDValue::get_value_vector(){
    return this -> value.val_vector;
}


/*!
*/
VCDReal      VCDValue::get_value_real(){
    return this -> value.val_real;
}

bool VCDValue::asBool() const {
    VCDBit b = this->type == VCD_SCALAR ? value.val_bit : asBit();
    return b == VCD_1;
}

VCDBit VCDValue::asBit() const {
    if(this->type == VCD_SCALAR){
        return this->value.val_bit;
    } else if (this->type == VCD_VECTOR){
        bool containOne = false;
        bool containX = false;
        for(auto b : *this->value.val_vector){
            switch (b) {
                case VCD_0:
                    break;
                case VCD_1:
                    containOne = true;
                    break;
                case VCD_X:
                    containX = true;
                    break;
                case VCD_Z:
                    return b;
            }
        }
        if(containX) return VCD_X;
        if(containOne) return VCD_1;
    }
    return VCD_0;
}

VCDValue* VCDValue::operator+(const VCDValue& that) const {
    //TODO
    auto* res = new VCDValue(VCD_0);
    return res;
}

VCDValue* VCDValue::operator&&(const VCDValue& that) const {
    VCDBit this_bit = this->asBit();
    VCDBit that_bit = that.asBit();
    VCDBit res_b;
    if(this_bit == VCD_Z || that_bit == VCD_Z){
        res_b = VCD_Z;
    } else if(this_bit == VCD_X || that_bit == VCD_X){
        res_b = VCD_X;
    } else {
        res_b = (this_bit == VCD_1) && (that_bit == VCD_1) ? VCD_1 : VCD_0;
    }
    auto* res = new VCDValue(res_b);
    return  res;
}

VCDValue *VCDValue::operator*(const VCDValue &that) const {
    return nullptr;
}

VCDValue *VCDValue::operator/(const VCDValue &that) const {
    return nullptr;
}

VCDValue *VCDValue::operator-(const VCDValue &that) const {
    return nullptr;
}

VCDValue *VCDValue::operator==(const VCDValue &that) const {
    return nullptr;
}

VCDValue *VCDValue::operator!=(const VCDValue &that) const {
    return nullptr;
}

VCDValue *VCDValue::operator||(const VCDValue &that) const {
    return nullptr;
}
