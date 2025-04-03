#ifndef BIT_MATH_H_
#define BIT_MATH_H_

#define MASK(x) (1<<x)

#define SET_BIT(REG,BIT) (REG |= MASK(BIT))
#define CLR_BIT(REG,BIT) (REG &= ~MASK(BIT))
#define TGL_BIT(REG,BIT) (REG ^= MASK(BIT))
#define GET_BIT(REG,BIT) ((REG>>BIT) & 1)





#endif