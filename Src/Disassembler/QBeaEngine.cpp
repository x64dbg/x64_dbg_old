#include "QBeaEngine.h"


QBeaEngine::QBeaEngine()
{
    /*
    QFile file("AsmCode.bin");

    if(file.open(QIODevice::ReadOnly) == false)
         qDebug() << "File has not been opened.";

    QByteArray data = file.readAll();
    qDebug() << "Size: " << data.size();

    if(data.size() == 0)
    {
        qDebug() << "No Data";
    }

    DISASM MyDisasm;
    int len, i = 0;
    int Error = 0;

    // Init the Disasm structure
    (void) memset (&MyDisasm, 0, sizeof(DISASM));

    // Init EIP
    MyDisasm.EIP = (UIntPtr)data.data();

    // Loop for Disasm
    while ((!Error) && (i<data.size()))
    {
        len = Disasm(&MyDisasm);
        if (len != UNKNOWN_OPCODE)
        {
            //(void) puts(MyDisasm.CompleteInstr);
            qDebug() << "i= " << (int)MyDisasm.EIP-(int)data.data() << QString(MyDisasm.CompleteInstr);
            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
            i += (int)len;
        }
        else {
            MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)1;
            i += (int)len;
        }
    }
    qDebug() << "End --------------------------------------------- i " << i;

    */



    /*

    // Init the Disasm structure
    DISASM MyDisasm;
    memset(&MyDisasm, 0, sizeof(DISASM));




//774F00C4   FF33                PUSH DWORD PTR DS:[EBX]
//774F00C6   53                  PUSH EBX
//774F00C7   68 D5004F90         PUSH ntdll.774F00D5                      ; ASCII "[Pj"
//774F00CC   FF7424 18           PUSH DWORD PTR SS:[ESP+18]
//774F00D0   E8 646C0300         CALL ntdll.RtlUnwind


    char bytes[] = {0xFF, 0x33, 0x53, 0x68, 0xD5, 0x00, 0x4F, 0x90, 0xFF, 0x74, 0x24, 0x18, 0xE8, 0x64, 0x6C, 0x03, 0x00};

    ulong prev = DisassembleBack(MyDisasm, bytes, (ulong)bytes, (ulong)17, (ulong)(bytes+8), 1);

    qDebug() << "cur : " << (ulong)(bytes+8) << "prev : " << (ulong)(prev);
    */


    // Init the Disasm structure
    memset(&mDisasmStruct, 0, sizeof(DISASM));
}




/**
 * @brief       Return the address of the nth instruction before the instruction pointed by ip.                 @n
 *              This function has been grab from OllyDbg ("Disassembleback" in asmserv.c)
 *
 * @param[in]   data    Pointer to the code data (Can be a buffer that is a copy of the original code)
 * @param[in]   base    RVA of the base (Same as data when data[] is a pointer to the original process memory)
 * @param[in]   size    Size of the memory page
 * @param[in]   ip      RVA of the current instruction (Original process memory)
 * @param[in]   n       Number of instruction back
 *
 * @return      Return the address of the nth instruction before the instruction pointed by ip
 */
ulong QBeaEngine::DisassembleBack(char* data, ulong base, ulong size, ulong ip, int n)
{
    int i;
    ulong abuf[131],addr,back,cmdsize;
    char* pdata;
    int len;

    // Check if the pointer is not null
    if (data == NULL)
    return 0;

    // Round the number of back instructions to 127
    if(n < 0)
    n = 0;
    else if (n > 127)
    n = 127;

    // Check if the instruction pointer ip is not outside the memory range
    if(ip > (base + size))
    ip = base + size;

    // Obvious answer
    if(n == 0)
    return ip;

    if(ip <= (base + n))
    return base;

    back = 16 * (n + 3); // Instruction length limited to 16

    if (ip < (base + back))
    back = ip - base;

    addr = ip - back;

    pdata = data + (addr - base);

    for (i=0; addr<ip; i++)
    {
        abuf[i%128]=addr;

        mDisasmStruct.EIP = (UIntPtr)pdata;
        len = Disasm(&mDisasmStruct);
        cmdsize = (len < 1) ? 1 : len ;

        pdata+=cmdsize;
        addr+=cmdsize;
        back-=cmdsize;
    }

    if (i < n)
        return abuf[0];
    else
        return abuf[(i - n + 128) % 128];
}




/**
 * @brief       Return the address of the nth instruction after the instruction pointed by ip.                 @n
 *              This function has been grab from OllyDbg ("Disassembleforward" in asmserv.c)
 *
 * @param[in]   data    Pointer to the code data (Can be a buffer that is a copy of the original code)
 * @param[in]   base    RVA of the base (Same as data when data[] is a pointer to the original process memory)
 * @param[in]   size    Size of the memory page
 * @param[in]   ip      RVA of the current instruction (Original process memory)
 * @param[in]   n       Number of instruction back
 *
 * @return      Return the address of the nth instruction after the instruction pointed by ip
 */
ulong QBeaEngine::DisassembleNext(char* data, ulong base, ulong size, ulong ip, int n)
{
    int i;
    ulong cmdsize;
    char *pdata;
    int len;

    if(data == NULL)
        return 0;

    if(ip < base)
        ip = base;

    if (ip > (base + size))
        ip = base + size;

    if(n <= 0)
        return ip;

    pdata = data + (ip - base);
    size -= (ip - base);

    for(i = 0; i < n && size > 0; i++)
    {
        mDisasmStruct.EIP = (UIntPtr)pdata;
        mDisasmStruct.SecurityBlock = (UIntPtr)size;
        len = Disasm(&mDisasmStruct);
        cmdsize = (len < 1) ? 1 : len ;

        pdata += cmdsize;
        ip += cmdsize;
        size -= cmdsize;
    }

    return ip;
}

Instruction_t QBeaEngine::DisassembleAt(char* data, ulong base, ulong size, ulong ip)
{
    long wInstRVA = (long)ip - (long)base;
    Instruction_t wInst;
    int len;

    mDisasmStruct.EIP = (UIntPtr)((long)data + (long)wInstRVA);
    mDisasmStruct.VirtualAddr = ip;
    mDisasmStruct.SecurityBlock = (UIntPtr)(size - wInstRVA);

    len = Disasm(&mDisasmStruct);
    len = (len < 1) ? 1 : len ;

    mDisasmStruct.SecurityBlock = 0;

    wInst.instStr = QString(mDisasmStruct.CompleteInstr);
    wInst.dump = QByteArray((char*)mDisasmStruct.EIP, len);
    wInst.rva = wInstRVA;
    wInst.lentgh = len;
    wInst.disasm = mDisasmStruct;

    return wInst;
}



