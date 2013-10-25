#include "BeaHighlight.h"

BeaHighlight::BeaHighlight()
{
}

bool BeaHighlight::PrintArgument(QList<CustomRichText_t>* richText, ARGTYPE* Argument, INSTRTYPE* Instruction, bool* had_arg)
{
    CustomRichText_t argument;
    argument.flags=FlagNone;
    int argtype=Argument->ArgType;
    int brtype=Instruction->BranchType;
    QString argmnemonic=QString(Argument->ArgMnemonic);
    if(argtype!=NO_ARGUMENT && *Argument->ArgMnemonic)
    {
        if(*had_arg) //add a comma
        {
            CustomRichText_t comma;
            comma.text=",";
            comma.flags=FlagNone;
            richText->push_back(comma);
        }

        if(argtype&MEMORY_TYPE) //mov [],a or mov a,[]
        {
            char segment[3]="";
            switch(Argument->SegmentReg)
            {
            case ESReg:
                strcpy(segment, "es");
                break;
            case DSReg:
                strcpy(segment, "ds");
                break;
            case FSReg:
                //TODO: x32 TEB
                strcpy(segment, "fs");
                break;
            case GSReg:
                //TODO: x64 TEB
                strcpy(segment, "gs");
                break;
            case CSReg:
                strcpy(segment, "cs");
                break;
            case SSReg:
                strcpy(segment, "ss");
                break;
            }
            int basereg=Argument->Memory.BaseRegister;
            if(basereg&REG4 || basereg&REG5) //esp or ebp
            {
                argument.textBackground=QColor(0,255,255);
                argument.flags=FlagBackground;
                //Highlight ESP or EBP memory move
            }
            else
            {
                argument.textColor=QColor(0,0,128);
                argument.flags=FlagColor;
            }

            //labels
            duint label_addr=Argument->Memory.Displacement;
            char label_text[MAX_LABEL_SIZE]="";
            if(DbgGetLabelAt(label_addr, label_text))
            {
                QString displacement;
                displacement.sprintf("%"fext"X", label_addr);
                argmnemonic.replace(displacement, QString(label_text));
            }

            switch(Argument->ArgSize)
            {
            case 8:
                argument.text.sprintf("byte ptr %s:[%s]", segment, argmnemonic.toUtf8().constData());
                break;
            case 16:
                argument.text.sprintf("word ptr %s:[%s]", segment, argmnemonic.toUtf8().constData());
                break;
            case 32:
                argument.text.sprintf("dword ptr %s:[%s]", segment, argmnemonic.toUtf8().constData());
                break;
            case 64:
                argument.text.sprintf("qword ptr %s:[%s]", segment, argmnemonic.toUtf8().constData());
                break;
            }
        }
        else
        {
            //labels
            duint label_addr=Instruction->Immediat;
            char label_text[MAX_LABEL_SIZE]="";
            if(DbgGetLabelAt(label_addr, label_text))
            {
                QString immediat;
                immediat.sprintf("%"fext"X", label_addr);
                argmnemonic.replace(immediat, QString(label_text));
            }

            char shortjmp[100]="\0";
            bool has_shortjmp=false;
            if(brtype and brtype!=RetType and !(argtype&REGISTER_TYPE))
            {
                argument.flags=FlagBackground;
                argument.textBackground=QColor(255,255,0);
                unsigned char* opc=(unsigned char*)&Instruction->Opcode;

                if(*opc==0xEB || Instruction->Opcode<0x80)
                {
                    strcpy(shortjmp+1, "short ");
                    has_shortjmp=true;
                    //short jumps
                }
                //Highlight (un)condentional jumps and calls
            }
            argument.text.sprintf("%s%s", shortjmp+has_shortjmp, argmnemonic.toUtf8().constData());
        }
        *had_arg=true;
        richText->push_back(argument);
    }
    else
        return false;
    return true;
}

void BeaHighlight::PrintBaseInstruction(QList<CustomRichText_t>* richText, DISASM* MyDisasm)
{
    CustomRichText_t mnemonic;
    char mnemonicText[16];
    strcpy(mnemonicText, MyDisasm->Instruction.Mnemonic);
    mnemonicText[strlen(mnemonicText)-1]=0; //remove space
    mnemonic.text=mnemonicText;
    int brtype=MyDisasm->Instruction.BranchType;
    if(brtype) //jump
    {
        if(brtype==RetType or brtype==CallType)
        {
            mnemonic.flags=FlagBackground;
            mnemonic.textBackground=QColor(0,255,255);
            //calls and rets
        }
        else if(brtype==JmpType)
        {
            mnemonic.flags=FlagBackground;
            mnemonic.textBackground=QColor(255,255,0);
            //uncond jumps
        }
        else
        {
            mnemonic.flags=FlagAll;
            mnemonic.textBackground=QColor(255,255,0);
            mnemonic.textColor=QColor(255,0,0);
            //cond jumps
        }
    }
    else if(!strcasecmp(mnemonicText, "push") or !strcasecmp(mnemonicText, "pop"))
    {
        mnemonic.flags=FlagColor;
        mnemonic.textColor=QColor(0,0,255);
        //push/pop
    }
    else if(!strcasecmp(mnemonicText, "nop"))
    {
        mnemonic.flags=FlagColor;
        mnemonic.textColor=QColor(128,128,128);
        //nop
    }
    else
        mnemonic.flags=FlagNone;
    richText->push_back(mnemonic);
}

void BeaHighlight::PrintRtfInstruction(QList<CustomRichText_t>* richText, DISASM* MyDisasm)
{
    CustomRichText_t space;
    space.text=" ";
    space.flags=FlagNone;

    CustomRichText_t prefix;
    int prefixCount=0;
    if(MyDisasm->Prefix.LockPrefix)
    {
        prefix.text+="lock ";
        prefixCount++;
    }
    if(MyDisasm->Prefix.RepPrefix)
    {
        prefix.text+="rep ";
        prefixCount++;
    }
    if(MyDisasm->Prefix.RepnePrefix)
    {
        prefix.text+="repne ";
        prefixCount++;
    }
    if(prefixCount)
    {
        prefix.flags=FlagNone; //no colors
        richText->push_back(prefix);
    }
    PrintBaseInstruction(richText, MyDisasm);
    richText->push_back(space);
    bool had_arg=false;
    PrintArgument(richText, &MyDisasm->Argument1, &MyDisasm->Instruction, &had_arg);
    PrintArgument(richText, &MyDisasm->Argument2, &MyDisasm->Instruction, &had_arg);
    PrintArgument(richText, &MyDisasm->Argument3, &MyDisasm->Instruction, &had_arg);
}
