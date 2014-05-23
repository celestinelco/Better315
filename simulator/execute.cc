#include "thumbsim.hpp"
#define PC_REG 15
#define LR_REG 14
#define SP_REG 13
#define PC rf[PC_REG]
#define LR rf[LR_REG]
#define SP rf[SP_REG]

unsigned int signExtend16to32ui(short i) {
  return static_cast<unsigned int>(static_cast<int>(i));
}

unsigned int signExtend8to32ui(char i) {
  return static_cast<unsigned int>(static_cast<int>(i));
}

ASPR flags;

// You're given the code for evaluating BEQ, 
// and you'll need to fill in the rest of these.
// See Page 99 of the armv6 manual
static int checkCondition(unsigned short cond) {
  switch(cond) {
    case EQ:
      if (flags.Z == 1) {
        return TRUE;
      }
      break;
    case NE:
      break;
    case CS:
      break;
    case CC:
      break;
    case MI:
      break;
    case PL:
      break;
    case VS:
      break;
    case VC:
      break;
    case HI:
      break;
    case LS:
      break;
    case GE:
      break;
    case LT:
      break;
    case GT:
      break;
    case LE:
      break;
    case AL:
      return TRUE;
      break;
  }
  return FALSE;
}

void execute() {
  Data16 instr = imem[PC];
  Thumb_Types itype;
  unsigned int pctarget = PC + 2;
  unsigned int addr;
  int diff, BitCount, bit;

  /* Convert instruction to correct type */
  ALU_Type alu(instr);
  SP_Type sp(instr);
  DP_Type dp(instr);
  LD_ST_Type ld_st(instr);
  MISC_Type misc(instr);
  COND_Type cond(instr);
  UNCOND_Type uncond(instr);
  LDM_Type ldm(instr);
  STM_Type stm(instr);
  LDRL_Type ldrl(instr);
  ADD_SP_Type addsp(instr);

  ALU_Ops add_ops;
  DP_Ops dp_ops;
  SP_Ops sp_ops;
  LD_ST_Ops ldst_ops;
  MISC_Ops misc_ops;

  rf.write(PC_REG, pctarget);

  itype = decode(ALL_Types(instr));
  switch(itype) {
    case ALU:
      add_ops = decode(alu);
      switch(add_ops) {
        case ALU_LSLI:
          break;
        case ALU_LSRI:
          break;
        case ALU_ASRI:
          break;
        case ALU_ADDR:
          rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
          break;
        case ALU_SUBR:
          break;
        case ALU_ADD3I:
          rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
          break;
        case ALU_SUB3I:
          break;
        case ALU_MOV:
          rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
          break;
        case ALU_CMP:
          break;
        case ALU_ADD8I:
          rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
          break;
        case ALU_SUB8I:
          break;
        default:
          break;
      }
      break;
    case DP:
      decode(dp);
      break;
    case SPECIAL:
      sp_ops = decode(sp);
      switch(sp_ops) {
        case SP_MOV:
          if (sp.instr.mov.d) {
            rf.write(SP_REG, rf[sp.instr.mov.rm]);
          }
          else {
            rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
          }
          break;
      }
      break;
    case LD_ST:
      // You'll want to use these load and store models
      // to implement ldrb/strb, ldm/stm and push/pop
      ldst_ops = decode(ld_st);
      switch(ldst_ops) {
        case STRR:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
          break;
        case LDRR:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
          break;
      }
      break;
    case MISC:
      misc_ops = decode(misc);
      switch(misc_ops) {
        case MISC_PUSH:
          break;
        case MISC_POP:
          break;
        case MISC_SUB:
          rf.write(SP_REG, SP - (misc.instr.sub.imm*4));
          break;
        case MISC_ADD:
          rf.write(SP_REG, SP + (misc.instr.add.imm*4));
          break;
      }
      break;
    case COND:
      decode(cond);
      // Once you've completed the checkCondition function,
      // this should work for all your conditional branches.
      if (checkCondition(cond.instr.b.cond)){
        rf.write(PC_REG, PC + 2 * signExtend8to32ui(cond.instr.b.imm) + 2);
      }
      break;
    case UNCOND:
      decode(uncond);
      break;
    case LDM:
      decode(ldm);
      break;
    case STM:
      decode(stm);
      break;
    case LDRL:
      decode(ldrl);
      break;
    case ADD_SP:
      decode(addsp);
      rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
      break;
    default:
      cout << "[ERROR] Unknown Instruction to be executed" << endl;
      exit(1);
      break;
  }
}
