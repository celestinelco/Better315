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
      if (flags.Z == 0) {
         return TRUE;
      }
      break;
    case CS:
      if (flags.C == 1) {
         return TRUE;
      }
      break;
    case CC:
      if (flags.C == 0) {
         return TRUE;
      }
      break;
    case MI:
      if (flags.N == 1) {
         return TRUE;
      }
      break;
    case PL:
      if (flags.N == 0) {
         return TRUE;
      }
      break;
    case VS:
      if (flags.V == 1) {
         return TRUE;
      }
      break;
    case VC:
      if (flags.V == 0) {
         return TRUE;
      }
      break;
    case HI:
      if (flags.C == 1 && flags.Z == 0) {
         return TRUE;
      }
      break;
    case LS:
      if (flags.C == 0 || flags.Z == 1) {
         return TRUE;
      }
      break;
    case GE:
      if (flags.N == flags.V) {
         return TRUE;
      }
      break;
    case LT:
      if (flags.N != flags.V) {
         return TRUE;
      }
      break;
    case GT:
      if (flags.Z == 0 && flags.N == flags.V) {
         return TRUE;
      }
      break;
    case LE:
      if (flags.Z == 1 || flags.N != flags.V) {
         return TRUE;
      }
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
  int x, y, result, sSum;
  unsigned int uSum;

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
          cout << "TODO: " << dec << __LINE__ << endl;
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] << alu.instr.lsli.imm);
          break;
        case ALU_LSRI:
          cout << "TODO: " << dec << __LINE__ << endl;
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] >> alu.instr.lsli.imm);
          break;
        case ALU_ASRI:
          cout << "TODO: " << dec << __LINE__ << endl;
          break;
        case ALU_ADDR:
          setFlags(rf[alu.instr.addr.rn], rf[alu.instr.addr.rm], OF_ADD);
          rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
          break;
        case ALU_SUBR:
          setFlags(rf[alu.instr.subr.rn], rf[alu.instr.subr.rm], OF_SUB);
          rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] - rf[alu.instr.subr.rm]);
          break;
        case ALU_ADD3I:
          setFlags(rf[alu.instr.add3i.rn], alu.instr.add3i.imm, OF_SUB);
          rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
          break;
        case ALU_SUB3I:
          setFlags(rf[alu.instr.sub3i.rn], alu.instr.sub3i.imm, OF_SUB);
          rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] - alu.instr.sub3i.imm);
          break;
        case ALU_MOV:
          setFlags(alu.instr.mov.imm, 0, OF_ADD);
          rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
          break;
        case ALU_CMP:
          setFlags(rf[alu.instr.cmp.rdn], alu.instr.cmp.imm, OF_SUB);
          break;
        case ALU_ADD8I:
          setFlags(rf[alu.instr.add8i.rdn], alu.instr.add8i.imm, OF_SUB);
          rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
          break;
        case ALU_SUB8I:
          setFlags(rf[alu.instr.sub8i.rdn], alu.instr.sub8i.imm, OF_SUB);
          rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] - alu.instr.sub8i.imm);
          break;
        default:
          break;
      }
      break;
    case DP:
      decode(dp);
      cout << "TODO: " << dec << __LINE__ << endl;
      break;
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
          caches.access(addr);
          stats.numMemWrites ++;
          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
          break;
        case LDRR:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          caches.access(addr);
          stats.numMemReads ++;
          rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
          break;
      }
      break;
    case MISC:
      misc_ops = decode(misc);
      switch(misc_ops) {
        case MISC_PUSH:
          addr = SP;
          for(BitCount = 0; BitCount < 8; BitCount ++) {
            // If we push this register
            if((1 << BitCount) & misc.instr.push.reg_list) {
              dmem.write(addr, rf[BitCount]);
              caches.access(addr);
              stats.numMemWrites ++;
              addr -= 4;
            }
          }
          if(misc.instr.push.m) {
            dmem.write(addr, LR);
            caches.access(addr);
            stats.numMemWrites ++;
            addr -= 4;
          }
          rf.write(SP_REG, addr);
          break;
        case MISC_POP:
          addr = SP;
          for(BitCount = 0; BitCount < 8; BitCount ++) {
            // If we push this register
            if(1 << BitCount & misc.instr.push.reg_list) {
              addr += 4;
              caches.access(addr);
              stats.numMemReads ++;
              rf.write(BitCount, dmem[addr]);
            }
          }
          if(misc.instr.push.m) {
            addr += 4;
            caches.access(addr);
            stats.numMemReads ++;
            rf.write(PC_REG, dmem[addr]);
          }
          rf.write(SP_REG, addr);
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
      int offset = 2 * signExtend8to32ui(cond.instr.b.imm) + 2;
      if (checkCondition(cond.instr.b.cond)){
        if (offset > 0)
          stats.numForwardBranchesTaken ++;
        else
          stats.numBackwardBranchesTaken ++;
        rf.write(PC_REG, PC + offset);
      }
      else {
        if (offset > 0)
          stats.numForwardBranchesNotTaken ++;
        else
          stats.numBackwardBranchesNotTaken ++;
      }
      break;
    case UNCOND:
      decode(uncond);
      rf.write(PC_REG, PC + 2 * signExtend8to32ui(uncond.instr.b.imm) + 2);
      break;
    case LDM:
      decode(ldm);
      addr = rf[ldm.instr.ldm.rn];
      for(BitCount = 0; BitCount < 8; BitCount ++) {
        // If we push this register
        if((1 << BitCount) & ldm.instr.ldm.reg_list) {
          addr += 4;
          rf.write(BitCount, dmem[addr]);
          caches.access(addr);
          stats.numMemReads ++;
        }
      }
      break;
    case STM:
      decode(stm);
      addr = rf[stm.instr.stm.rn];
      for(BitCount = 0; BitCount < 8; BitCount ++) {
        // If we push this register
        if(1 << BitCount & stm.instr.stm.reg_list) {
          dmem.write(addr, rf[BitCount]);
          caches.access(addr);
          stats.numMemWrites ++;
          addr -= 4;
        }
      }
      break;
    case LDRL:
      cout << "PC: " << PC << endl;
      decode(ldrl);
      addr = PC + ldrl.instr.ldrl.imm * 2;
      rf.write(ldrl.instr.ldrl.rt, signExtend16to32ui(imem[addr]));
      cout << "r" << ldrl.instr.ldrl.rt << ": " << hex << rf[ldrl.instr.ldrl.rt] << endl;
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

  stats.instrs ++;
}

void setFlags(int num1, int num2, OFType oftype) {
   int result;
   switch(oftype) {
   case OF_ADD:
      result = num1 + num2;
      break;
   case OF_SUB:
      result = num1 - num2;
      break;
   case OF_SHIFT:
      result = num1 << num2;
      break;
   default:
      cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
      return(1);
   }

   flags.Z = result == 0;
   flags.N = result < 0;

   setCarryOverflow(num1, num2, oftype);
}

void setCarryOverflow (int num1, int num2, OFType oftype) {
  switch (oftype) {
    case OF_ADD:
      if (((unsigned long long int)num1 + (unsigned long long int)num2) ==
          ((unsigned int)num1 + (unsigned int)num2)) {
        flags.C = 0;
      }
      else {
        flags.C = 1;
      }
      if (((long long int)num1 + (long long int)num2) ==
          ((int)num1 + (int)num2)) {
        flags.V = 0;
      }
      else {
        flags.V = 1;
      }
      break;
    case OF_SUB:
      if (((unsigned long long int)num1 - (unsigned long long int)num2) ==
          ((unsigned int)num1 - (unsigned int)num2)) {
        flags.C = 0;
      }
      else {
        flags.C = 1;
      }
      if (((num1==0) && (num2==0)) || 
          (((long long int)num1 - (long long int)num2) ==
          ((int)num1 - (int)num2))) {
        flags.V = 0;
      }
      else {
        flags.V = 1;
      }
      break;
    case OF_SHIFT:
      // C flag unaffected for shifts by zero
      if (num2 != 0) {
        if (((unsigned long long int)num1 << (unsigned long long int)num2) ==
            ((unsigned int)num1 << (unsigned int)num2)) {
          flags.C = 0;
        }
        else {
          flags.C = 1;
        }
      }
      // Shift doesn't set overflow
      break;
    default:
      cerr << "Bad OverFlow Type encountered." << __LINE__ << __FILE__ << endl;
      exit(1);
  }
}
