#ifndef _bit_op_h_
#define _bit_op_h_

#define set_bit(val, idx) (val |= (1ul << idx))
#define clear_bit(val, idx) (val &= ~(1ul << idx))
#define get_bit(val, idx) (!!(val & (1ul << idx)))

#endif