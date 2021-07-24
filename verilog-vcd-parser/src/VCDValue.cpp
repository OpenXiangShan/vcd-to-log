
#include "VCDValue.hpp"
#include <iostream>
#include "assert.h"

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

int CntOne(bool c_in, VCDBit op1 , VCDBit op2)
{
    int cnt = 0;
    if(c_in) cnt++; if(op1 == VCD_1) cnt++; if(op2 == VCD_1) cnt++;
    return cnt;
}
VCDBitVector *VCDValue::Reverse(VCDBitVector* in_vector)
{
    VCDBitVector* res = new VCDBitVector();
    for(auto it = in_vector->begin();it != in_vector->end();it++)
    {
        if(*it == VCD_1)
        {
            res->push_back(VCD_0);
        }else
        {
            res->push_back(VCD_1);
        }
        
    }
    return res;
}

VCDValue* VCDValue::operator+(const VCDValue& that) const {
    //TODO
    // auto* res = new VCDValue(VCD_0);
    // return res;
    if(this -> type == VCD_SCALAR && that.type == VCD_SCALAR)
    {
        if(this -> value.val_bit == VCD_1 && that.value.val_bit == VCD_1)
        {
            auto* res = new VCDBitVector();
            res->push_back(VCD_1);
            res->push_back(VCD_0);
            return new VCDValue(res);
        }else if(this -> value.val_bit == VCD_0 && that.value.val_bit == VCD_0)
        {
            return new VCDValue(VCD_0);
        }else
        {
            return new VCDValue(VCD_1);
        }
        

    }else if(this -> type == VCD_VECTOR && that.type == VCD_VECTOR)
    {
        int res_len = this->value.val_vector->size() > that.value.val_vector->size() ? this->value.val_vector->size() : that.value.val_vector->size() ;
        VCDBitVector *res = new VCDBitVector(res_len + 1,VCD_0);
        bool c_in = false;
        int this_ptr = this->value.val_vector->size()-1;
        int that_ptr = that.value.val_vector->size()-1;
        int res_ptr = res->size()-1;
        // std::cout << "this:";
        // for(auto it : * this->value.val_vector)
        // {
        //     std::cout << VCDValue::VCDBit2Char(it);
        // }
        // std::cout << std::endl;
        // std::cout << "that:";
        // for(auto it :  * that.value.val_vector)
        // {
        //     std::cout << VCDValue::VCDBit2Char(it);
        // }
        // std::cout << std::endl;
        while(this_ptr >= 0 && that_ptr >= 0)
        {
            int temp = CntOne(c_in,(* this->value.val_vector)[this_ptr],(* that.value.val_vector)[that_ptr]);
            if(temp >= 2)
            {
                c_in = true;
            }else
            {
                c_in = false;
            }
            if(temp == 1 || temp == 3)
            {
                (*res)[res_ptr] = VCD_1;
            }else
            {
                (*res)[res_ptr] = VCD_0;
            }
            
            this_ptr -- ;
            that_ptr -- ;
            res_ptr -- ;
        }
        while(this_ptr >= 0)
        {
            int temp = CntOne(c_in,(this->value.val_vector)->at(this_ptr),VCD_0);
            if(temp >= 2)
            {
                c_in = true;
            }else
            {
                c_in = false;
            }
            if(temp == 1 || temp == 3)
            {
                (*res)[res_ptr] = VCD_1;
            }else
            {
                (*res)[res_ptr] = VCD_0;
            }
            
            this_ptr -- ;
            res_ptr -- ;
        }
        while(that_ptr >= 0)
        {
            int temp = CntOne(c_in,VCD_0,(that.value.val_vector)->at(that_ptr));
            if(temp >= 2)
            {
                c_in = true;
            }else
            {
                c_in = false;
            }
            if(temp == 1 || temp == 3)
            {
                (*res)[res_ptr] = VCD_1;
            }else
            {
                (*res)[res_ptr] = VCD_0;
            }
            that_ptr -- ;
            res_ptr -- ;
        }
        (*res)[res_ptr] = c_in ? VCD_1 : VCD_0;
        // std::cout << "res:";
        // for(auto it : *res)
        // {
        //     std::cout << VCDValue::VCDBit2Char(it);
        // }
        // std::cout << std::endl;
        return new VCDValue(res);

    }else 
    {
        if(this->type == VCD_SCALAR)
        {
            if(this->value.val_bit == VCD_0)
            {
                return new VCDValue(that.value.val_vector);
            }else
            {
                VCDBitVector* temp = new VCDBitVector();
                temp->push_back(VCD_1);
                return VCDValue(temp) + that;
            }
            
        }else
        {
            if(that.value.val_bit == VCD_0)
            {
                return new VCDValue(this->value.val_vector);
            }else
            {
                VCDBitVector* temp = new VCDBitVector();
                temp->push_back(VCD_1);
                return *this + VCDValue(temp);
            }
            
        }
        
    }
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
    // return nullptr;
    if(this -> type == VCD_SCALAR && that.type == VCD_SCALAR)
    {   
        if(this -> value.val_bit == VCD_1 && that.value.val_bit == VCD_1)
        {
            return new VCDValue(VCD_1);
        }else
        {
            return new VCDValue(VCD_0);
        }
    }else if(this -> type == VCD_VECTOR && that.type == VCD_VECTOR)
    {
        VCDValue* op1 = new VCDValue(new VCDBitVector());
        VCDValue* res = new VCDValue(new VCDBitVector());
        for(VCDBit item : * this->value.val_vector)
        {
            op1->value.val_vector->push_back(item);
        }
        res->value.val_vector->push_back(VCD_0);

        for(auto it = (that.value.val_vector->end() - 1);it != (that.value.val_vector->begin() - 1);it--)
        {
            if(*it == VCD_1)
            {
                res = *res + *op1;
                op1->value.val_vector->push_back(VCD_0);
            }else
            {
                op1->value.val_vector->push_back(VCD_0);
            }
            
        }
        return res;
    }else 
    {
        if(this->type == VCD_SCALAR)
        {
            if(this->value.val_bit == VCD_1)
            {
                return new VCDValue(that.value.val_vector);
            }else
            {
                return new VCDValue(VCD_0);
            }
            
        }else
        {
            if(that.value.val_bit == VCD_1)
            {
                return new VCDValue(this->value.val_vector);
            }else
            {
                return new VCDValue(VCD_0);
            }
        }
        
    }
}

VCDValue *VCDValue::operator/(const VCDValue &that) const {
    return nullptr;
    // TODO: /
    // if(this -> type == VCD_SCALAR && that.type == VCD_SCALAR)
    // {
    //     assert(that.value.val_bit != VCD_0 && "Divide by 0");
    //     if(this -> value.val_bit == VCD_0)
    //     {
    //         return new VCDValue(VCD_0);
    //     }else
    //     {
    //         return new VCDValue(VCD_1);
    //     }
        
    // }else if(this -> type == VCD_VECTOR && that.type == VCD_VECTOR)
    // {
        
    // }else 
    // {
        
    // }
}

VCDValue *VCDValue::operator-(const VCDValue &that) const {
    int cnt_zero_left = this->value.val_vector->size() - that.value.val_vector->size();
    VCDBitVector* op2 = new VCDBitVector();

    //add
    for(int i=0;i<cnt_zero_left;i++)
    {
        op2->push_back(VCD_0);
    }
    for(VCDBit item : * that.value.val_vector)
    {
        op2->push_back(item);
    }

    VCDValue* _op2 = VCDValue(Reverse(op2)) + VCDValue(VCD_1);
    _op2->value.val_vector->erase(_op2->value.val_vector->begin());

    VCDValue* res = *this + *_op2;
    res->value.val_vector->erase(res->value.val_vector->begin());

    return res;
}

VCDValue *VCDValue::operator!=(const VCDValue &that) const {
    auto temp = (*this == that);
    if(temp->asBool())
    {
        return new VCDValue(VCD_0);
    }else
    {
        return new VCDValue(VCD_1);
    }
    
}

VCDValue *VCDValue::operator==(const VCDValue &that) const {
    // return nullptr;
    if(this -> type == VCD_SCALAR && that.type == VCD_SCALAR)
    {
        return this->asBool() == that.asBool() ? new VCDValue(VCD_1) : new VCDValue(VCD_0);
    }else if(this -> type == VCD_VECTOR && that.type == VCD_VECTOR)
    {
        int smaller_size = (this->value).val_vector -> size() >= that.value.val_vector -> size() ?  that.value.val_vector -> size()   : (this->value).val_vector -> size();
        int bigger_size  = (this->value).val_vector -> size() >= that.value.val_vector -> size() ? (this->value).val_vector -> size() : that.value.val_vector -> size();
        int index = (this->value).val_vector -> size() >= that.value.val_vector -> size() ? 1 : 0; // 1:this ; 0:that
        int cnt = 0;
        auto it_this = (this->value).val_vector -> end() - 1 ; auto it_that = that.value.val_vector -> end() - 1;
        for(; cnt < smaller_size ; cnt++ , it_this-- , it_that--)
        {
            if(*it_this != *it_that)
            {
                return new VCDValue(VCD_0);
            }
        }
        if(index == 1)
        {
            for(; cnt < bigger_size ; cnt++ , it_this--)
            {
                if(*it_this !=  VCD_0)
                {
                    return new VCDValue(VCD_0);
                }
            }
            
        }else
        {
            for(; cnt < bigger_size ; cnt++ , it_that--)
            {
                if(*it_that !=  VCD_0)
                {
                    return new VCDValue(VCD_0);
                }
            }
        }
        return new VCDValue(VCD_1);
    }else
    {
        VCDBit op1_bit = this -> type == VCD_SCALAR ? this -> value.val_bit : that.value.val_bit;
        VCDBitVector* op2_bits = this -> type == VCD_SCALAR ? that.value.val_vector : this->value.val_vector;

        if(*(op2_bits->end() - 1) != op1_bit)
        {
            return new VCDValue(VCD_0);
        }
        for(auto it=op2_bits->begin();it!=(op2_bits->end() - 1);it++)
        {
            if(*it == VCD_1)
            {
                return new VCDValue(VCD_0);
            }
        }
        return new VCDValue(VCD_1);
    }
}

VCDValue *VCDValue::operator||(const VCDValue &that) const {
    // return nullptr;
    VCDBit this_bit = this -> asBit();
    VCDBit that_bit = that.asBit();
    VCDBit res_b;
    if(this_bit == VCD_Z || that_bit == VCD_Z){
        res_b = VCD_Z;
    } else if(this_bit == VCD_X || that_bit == VCD_X){
        res_b = VCD_X;
    } else {
        res_b = (this_bit == VCD_1) || (that_bit == VCD_1) ? VCD_1 : VCD_0;
    }
    auto* res = new VCDValue(res_b);
    return  res;

}

VCDValue *VCDValue::operator|(const VCDValue &that) const {
    if(this->type == VCD_SCALAR && that.type == VCD_SCALAR)
    {
        if(this->value.val_bit == VCD_1 || that.value.val_bit == VCD_1)
        {
            return new VCDValue(VCD_1);
        }else
        {
            return new VCDValue(VCD_0);
        }

    }else if(this->type == VCD_VECTOR && that.type == VCD_VECTOR)
    {   
        int max_len = this->value.val_vector->size() > that.value.val_vector->size() ? this->value.val_vector->size() : that.value.val_vector->size();
        VCDBitVector* res = new VCDBitVector(max_len,VCD_0);

        int this_ptr = this->value.val_vector->size() - 1;
        int that_ptr = that.value.val_vector->size() - 1;
        int res_ptr = max_len - 1;

        while(this_ptr >= 0 && that_ptr >= 0)
        {
            if((*this->value.val_vector)[this_ptr] == VCD_1 || (*that.value.val_vector)[that_ptr] == VCD_1)
            {
                (*res)[res_ptr] = VCD_1;
            }else
            {
                (*res)[res_ptr] = VCD_0;
            }
            
            this_ptr--;
            that_ptr--;
            res_ptr--;
        }
        while(this_ptr >= 0)
        {
            (*res)[res_ptr] = (*this->value.val_vector)[this_ptr];
            this_ptr--;
            res_ptr--;
        }
        while(that_ptr >= 0)
        {
            (*res)[res_ptr] = (*that.value.val_vector)[that_ptr];
            that_ptr--;
            res_ptr--;
        }

        return new VCDValue(res);
    }else
    {
        if(this->type == VCD_SCALAR)
        {
            VCDBitVector* op1 = new VCDBitVector();
            op1->push_back(this->value.val_bit);

            return VCDValue(op1) | that;

        }else
        {
            VCDBitVector* op2 = new VCDBitVector();
            op2->push_back(that.value.val_bit);

            return *this | VCDValue(op2);
        }
        
    }
    
    
}
VCDValue *VCDValue::operator&(const VCDValue &that) const {
    if(this->type == VCD_SCALAR && that.type == VCD_SCALAR)
    {
        if(this->value.val_bit == VCD_1 && that.value.val_bit == VCD_1)
        {
            return new VCDValue(VCD_1);
        }else
        {
            return new VCDValue(VCD_0);
        }

    }else if(this->type == VCD_VECTOR && that.type == VCD_VECTOR)
    {   
        int max_len = this->value.val_vector->size() > that.value.val_vector->size() ? this->value.val_vector->size() : that.value.val_vector->size();
        VCDBitVector* res = new VCDBitVector(max_len,VCD_0);

        int this_ptr = this->value.val_vector->size() - 1;
        int that_ptr = that.value.val_vector->size() - 1;
        int res_ptr = max_len - 1;

        while(this_ptr >= 0 && that_ptr >= 0)
        {
            if((*this->value.val_vector)[this_ptr] == VCD_1 && (*that.value.val_vector)[that_ptr] == VCD_1)
            {
                (*res)[res_ptr] = VCD_1;
            }else
            {
                (*res)[res_ptr] = VCD_0;
            }
            
            this_ptr--;
            that_ptr--;
            res_ptr--;
        }
        while(this_ptr >= 0)
        {
            (*res)[res_ptr] = VCD_0;
            this_ptr--;
            res_ptr--;
        }
        while(that_ptr >= 0)
        {
            (*res)[res_ptr] = VCD_0;
            that_ptr--;
            res_ptr--;
        }

        return new VCDValue(res);
    }else
    {
        if(this->type == VCD_SCALAR)
        {
            VCDBitVector* op1 = new VCDBitVector();
            op1->push_back(this->value.val_bit);

            return VCDValue(op1) & that;

        }else
        {
            VCDBitVector* op2 = new VCDBitVector();
            op2->push_back(that.value.val_bit);

            return *this & VCDValue(op2);
        }
        
    }
    
}
