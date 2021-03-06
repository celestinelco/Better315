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
  Data16 instr = imem[PC], instr2;
  Thumb_Types itype;
  unsigned int pctarget = PC + 2;
  unsigned int addr;
  int BitCount;
  int result;
  int offset;

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
  BL_Type blupper(instr);

  ALU_Ops add_ops;
  DP_Ops dp_ops;
  SP_Ops sp_ops;
  LD_ST_Ops ldst_ops;
  MISC_Ops misc_ops;
  BL_Ops bl_ops;

  rf.write(PC_REG, pctarget);
  stats.numRegWrites ++;

  itype = decode(ALL_Types(instr));
  switch(itype) {
    case ALU:
      add_ops = decode(alu);
      switch(add_ops) {
        case ALU_LSLI:
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] << alu.instr.lsli.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        case ALU_LSRI:
          rf.write(alu.instr.lsli.rd, rf[alu.instr.lsli.rm] >> alu.instr.lsli.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        case ALU_ASRI:
          cout << "TODO: " << dec << __LINE__ << endl;
          break;
        case ALU_ADDR:
          setFlags(rf[alu.instr.addr.rn], rf[alu.instr.addr.rm], OF_ADD);
          rf.write(alu.instr.addr.rd, rf[alu.instr.addr.rn] + rf[alu.instr.addr.rm]);
          stats.numRegReads += 2;
          stats.numRegWrites ++;
          break;
        case ALU_SUBR:
          setFlags(rf[alu.instr.subr.rn], rf[alu.instr.subr.rm], OF_SUB);
          rf.write(alu.instr.subr.rd, rf[alu.instr.subr.rn] - rf[alu.instr.subr.rm]);
          stats.numRegReads += 2;
          stats.numRegWrites ++;
          break;
        case ALU_ADD3I:
          setFlags(rf[alu.instr.add3i.rn], alu.instr.add3i.imm, OF_SUB);
          rf.write(alu.instr.add3i.rd, rf[alu.instr.add3i.rn] + alu.instr.add3i.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        case ALU_SUB3I:
          setFlags(rf[alu.instr.sub3i.rn], alu.instr.sub3i.imm, OF_SUB);
          rf.write(alu.instr.sub3i.rd, rf[alu.instr.sub3i.rn] - alu.instr.sub3i.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        case ALU_MOV:
          setFlags(alu.instr.mov.imm, 0, OF_ADD);
          rf.write(alu.instr.mov.rdn, alu.instr.mov.imm);
          stats.numRegWrites ++;
          break;
        case ALU_CMP:
          setFlags(rf[alu.instr.cmp.rdn], alu.instr.cmp.imm, OF_SUB);
          stats.numRegReads ++;
          break;
        case ALU_ADD8I:
          setFlags(rf[alu.instr.add8i.rdn], alu.instr.add8i.imm, OF_SUB);
          rf.write(alu.instr.add8i.rdn, rf[alu.instr.add8i.rdn] + alu.instr.add8i.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        case ALU_SUB8I:
          setFlags(rf[alu.instr.sub8i.rdn], alu.instr.sub8i.imm, OF_SUB);
          rf.write(alu.instr.sub8i.rdn, rf[alu.instr.sub8i.rdn] - alu.instr.sub8i.imm);
          stats.numRegReads ++;
          stats.numRegWrites ++;
          break;
        default:
          break;
      }
      break;
    case DP:
      dp_ops = decode(dp);
      switch(dp_ops) {
      case DP_CMP:
         setFlags(rf[dp.instr.DP_Instr.rdn], rf[dp.instr.DP_Instr.rm], OF_SUB);
         stats.numRegReads += 2;
         break; 
      default:
         cout << "TODO: DP(" << dp_ops << ")" << endl;
      }
      break;
    case SPECIAL:
      sp_ops = decode(sp);
      switch(sp_ops) {
        case SP_MOV:
          if (sp.instr.mov.d) {
            rf.write(SP_REG, rf[sp.instr.mov.rm]);
            stats.numRegReads ++;
            stats.numRegWrites ++;
          }
          else {
            rf.write(sp.instr.mov.rd, rf[sp.instr.mov.rm]);
            stats.numRegReads ++;
            stats.numRegWrites ++;
          }
          break;
      }
      break;
    case LD_ST:
      // You'll want to use these load and store models
      // to implement ldrb/strb, ldm/stm and push/pop
      ldst_ops = decode(ld_st);
      switch(ldst_ops) {
        case STRI:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          caches.access(addr);
          stats.numRegReads ++;
          stats.numMemWrites ++;

          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
          stats.numRegReads ++;
          break;
        case LDRI:
          addr = rf[ld_st.instr.ld_st_imm.rn] + ld_st.instr.ld_st_imm.imm * 4;
          caches.access(addr);
          stats.numRegReads ++;
          stats.numMemReads ++;

          rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr]);
          stats.numRegWrites ++;
          break;
        case LDRBR:
          addr = rf[ld_st.instr.ld_st_reg.rm] + rf[ld_st.instr.ld_st_reg.rn];
          caches.access(addr);
          stats.numRegReads += 2;

          rf.write(ld_st.instr.ld_st_imm.rt, dmem[addr].data_ubyte4(0));
          stats.numMemReads ++;
          stats.numRegWrites ++;
          break;
        case STRBR:
          addr = rf[ld_st.instr.ld_st_reg.rm] + rf[ld_st.instr.ld_st_reg.rn];
          caches.access(addr);
          stats.numRegReads += 2;

          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt] & 0xff);
          stats.numMemWrites ++;
          stats.numRegReads ++;
          break;
        case STRR:
          addr = rf[ld_st.instr.ld_st_reg.rn] + rf[ld_st.instr.ld_st_reg.rm];
          caches.access(addr);
          stats.numRegReads ++;
          stats.numMemWrites ++;

          dmem.write(addr, rf[ld_st.instr.ld_st_imm.rt]);
          stats.numRegReads ++;
          break;
        default:
          cout << "TODO: " << ldst_ops << endl;
      }
      break;
    case MISC:
      misc_ops = decode(misc);
      switch(misc_ops) {
        case MISC_PUSH:
          addr = SP;
          stats.numRegReads ++;
          for(BitCount = 0; BitCount < 8; BitCount ++) {
            // If we push this register
            if((1 << BitCount) & misc.instr.push.reg_list) {
              dmem.write(addr, rf[BitCount]);
              caches.access(addr);
              stats.numRegReads ++;
              stats.numMemWrites ++;
              addr -= 4;
            }
          }
          if(misc.instr.push.m) {
            dmem.write(addr, LR);
            caches.access(addr);
            stats.numRegReads ++;
            stats.numMemWrites ++;
            addr -= 4;
          }
          rf.write(SP_REG, addr);
          stats.numRegWrites ++;
          break;
        case MISC_POP:
          addr = SP;
          stats.numRegReads ++;
          if(misc.instr.push.m) {
            addr += 4;
            caches.access(addr);
            rf.write(PC_REG, dmem[addr]);
            stats.numMemReads ++;
            stats.numRegWrites ++;
          }
          for(BitCount = 7; BitCount >= 0; BitCount --) {
            // If we push this register
            if(1 << BitCount & misc.instr.push.reg_list) {
              addr += 4;
              caches.access(addr);
              stats.numMemReads ++;
              rf.write(BitCount, dmem[addr]);
              stats.numRegWrites ++;
            }
          }
          rf.write(SP_REG, addr);
          stats.numRegWrites ++;
          break;
        case MISC_SUB:
          rf.write(SP_REG, SP - (misc.instr.sub.imm*4));
          stats.numRegWrites ++;
          break;
        case MISC_ADD:
          rf.write(SP_REG, SP + (misc.instr.add.imm*4));
          stats.numRegWrites ++;
          break;
      }
      break;
    case COND:
      decode(cond);
      // Once you've completed the checkCondition function,
      // this should work for all your conditional branches.
      offset = 2 * signExtend8to32ui(cond.instr.b.imm) + 2;
      if (checkCondition(cond.instr.b.cond)){
        if (offset > 0)
          stats.numForwardBranchesTaken ++;
        else
          stats.numBackwardBranchesTaken ++;
        rf.write(PC_REG, PC + offset);
        stats.numRegWrites ++;
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
      stats.numRegWrites ++;
      break;
    case LDM:
      decode(ldm);
      addr = rf[ldm.instr.ldm.rn];
      stats.numRegReads ++;
      for(BitCount = 0; BitCount < 8; BitCount ++) {
        // If we push this register
        if((1 << BitCount) & ldm.instr.ldm.reg_list) {
          addr += 4;
          rf.write(BitCount, dmem[addr]);
          stats.numRegWrites ++;
          caches.access(addr);
          stats.numMemReads ++;
        }
      }
      break;
    case STM:
      decode(stm);
      addr = rf[stm.instr.stm.rn];
      stats.numRegReads ++;
      for(BitCount = 0; BitCount < 8; BitCount ++) {
        // If we push this register
        if(1 << BitCount & stm.instr.stm.reg_list) {
          dmem.write(addr, rf[BitCount]);
          stats.numRegReads ++;
          caches.access(addr);
          stats.numMemWrites ++;
          addr += 4;
        }
      }
      rf.write(stm.instr.stm.rn, addr);
      stats.numRegWrites ++;
      break;
    case LDRL:
      decode(ldrl);
      addr = PC + ldrl.instr.ldrl.imm * 4 - 2;
      rf.write(ldrl.instr.ldrl.rt, signExtend16to32ui(imem[addr]));
      stats.numRegWrites ++;
      break;
    case ADD_SP:
      decode(addsp);
      rf.write(addsp.instr.add.rd, SP + (addsp.instr.add.imm*4));
      stats.numRegWrites ++;
      break;
    case BL:
      bl_ops = decode(blupper);
      if (bl_ops == BL_UPPER) {
        // PC has already been incremented above
        instr2 = imem[PC];
        BL_Type bllower(instr2);
        if (blupper.instr.bl_upper.s) {
          addr = static_cast<int>(0xff<<24) | 
                 ((~(bllower.instr.bl_lower.j1 ^ blupper.instr.bl_upper.s))<<23) | 
                 ((~(bllower.instr.bl_lower.j2 ^ blupper.instr.bl_upper.s))<<22) | 
                 ((blupper.instr.bl_upper.imm10)<<12) | 
                 ((bllower.instr.bl_lower.imm11)<<1);
        }
        else { 
          addr = ((blupper.instr.bl_upper.imm10)<<12) | 
                 ((bllower.instr.bl_lower.imm11)<<1); 
        }
        // return address is 4-bytes away from the start of the BL insn 
        rf.write(LR_REG, PC + 2); 
        stats.numRegWrites ++;
        // Target address is also computed from that point 
        rf.write(PC_REG, PC + 2 + addr); 
        stats.numRegWrites ++;
      }
      else {
        cerr << "Bad BL format." << endl; exit(1); 
      }
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
