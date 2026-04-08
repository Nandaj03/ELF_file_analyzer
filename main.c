#include<stdio.h>
#include<stdlib.h>
#include<elf.h>

int main()
{
    char file_name[100];
    printf("enter the file name:");
    scanf("%99[^\n]",file_name);
    FILE *fp;
    fp=fopen(file_name,"rb");
    if(fp==NULL)
    {
        fprintf(stderr,"file not found\n");
        return 0;
    }
    printf("File opened successfully\n");

    unsigned char buffer[16];
    if(fread(buffer,1,16,fp)!=16)
    {
        printf("16 bytes reading unsuccessful\n");
        fclose(fp);
        return 0;
    }
    if(buffer[0]== 0x7F && buffer[1]==0x45 && buffer[2]==0x4c && buffer[3]==0x46)
    {
        printf("%-20s= %-20s\n","File type","ELF magic detected");
    }
    else {
        printf("Not a Elf file\n");
        fclose(fp);
        return 0;
    }

    if(buffer[4]==0x01) printf("%-20s=%-20s\n","Bit","32");
    else if(buffer[4]==0x02) printf("%-20s=%-20s\n","Bit","64");
    else if(buffer[4]==0) printf("NONE(INVALID)\n");

    if(buffer[5]==0x01) printf("%-20s=%-20s\n","Endian","Little Endian");
    else if(buffer[5]==0x02) printf("%-20s=%-20s\n","Endian","Big Endian");
    else printf("NONE\n");

    printf("\n--------------------------------------------------------------\n");
    printf("ELF header Details\n");
    printf("--------------------------------------------------------------");
    fseek(fp,0,SEEK_SET);
    if(buffer[4]==0x02)
    {
        Elf64_Ehdr ehdr;
        if((fread(&ehdr,sizeof(ehdr),1,fp))!=1)
        {
            fprintf(stderr,"cannot read ELF header\n");
            fclose(fp);
            return 0;
        }
        printf("\n%-20s=%u\n","Type",ehdr.e_type);
        printf("%-20s=%u\n","Machine",ehdr.e_machine);
        printf("%-20s=%u\n","Version",ehdr.e_version);
        printf("%-20s=0x%lx\n","Entry point",ehdr.e_entry);
        printf("%-20s=%lu\n","pgm header offset",ehdr.e_phoff);
        printf("%-20s=%lu\n","section header offset",ehdr.e_shoff);
        printf("%-20s=%u\n","pgrm header count",ehdr.e_phnum);
        printf("%-20s=%u\n","section header count",ehdr.e_shnum);

    char ch;
    printf("\nDou want to print program header details:(y/n)\n");
    scanf(" %c",&ch);
    if(ch=='n' || ch=='N') return 0;    
    printf("------------------------------------------------\n");
    printf("Program header details:\n");
    printf("------------------------------------------------\n");

    fseek(fp,ehdr.e_phoff,SEEK_SET);
    for(int i=0;i<ehdr.e_phnum;i++)
    {
        Elf64_Phdr phdr;

        if(fread(&phdr,ehdr.e_phentsize,1,fp)!=1)
            break;
        printf("--------------------\n");
        printf("pgmheader-->%d     |\n",i+1);
        printf("--------------------\n");
        printf("Type=%u\n",phdr.p_type);
        printf("offset=0x%lx\n",phdr.p_offset);
        printf("virtual address=0x%lx\n",phdr.p_vaddr);
        printf("file size=%lu\n",phdr.p_filesz);
        printf("memory size=%lu\n",phdr.p_memsz);
        printf("\n");
        
    }

    char ch1;
    printf("\n do you want to print section header:(y/n)");
    scanf(" %c",&ch1);
    if(ch1=='n' || ch=='N') return 0;

    printf("-------------------------------\n");
    printf("Section header details:\n");
    printf("-------------------------------\n");
    fseek(fp,ehdr.e_shoff,SEEK_SET);

    for(int i=0;i<ehdr.e_shnum;i++)
    {
        Elf64_Shdr shdr;
        if(fread(&shdr,ehdr.e_shentsize,1,fp)!=1)
            break;
        printf("-------------------------\n");
        printf("section header-->%d    |\n",i+1);
        printf("-------------------------\n");
        printf("type=%u\n",shdr.sh_type);
        printf("offset=0x%lx\n",shdr.sh_offset);
        printf("size=%lu\n",shdr.sh_size);
        printf("address=0x%lu\n",shdr.sh_addr);
    }
    

    }
    else if(buffer[4]==0x01)
    {
        Elf32_Ehdr ehdr;
        if((fread(&ehdr,sizeof(ehdr),1,fp))!=1)
        {
            fprintf(stderr,"cannot read ELF header\n");
            fclose(fp);
            return 0;
        }
        printf("\n%-10s=%u\n","Type",ehdr.e_type);
        printf("Machine=%u \n",ehdr.e_machine);
        printf("version=%u\n",ehdr.e_version);
        printf("entry point=0x%u\n",ehdr.e_entry);
        printf("pgm header offset=%u\n",ehdr.e_phoff);
        printf("section header offset=%u\n",ehdr.e_shoff);
        printf("pgrm header=%u\n",ehdr.e_phnum);
        printf("section header=%u\n",ehdr.e_shnum);

    char ch;
    printf("\n do you want to print section header:(y/n)");
    scanf(" %c",&ch);
    if(ch=='n' || ch=='N') return 0;

    printf("------------------------------------------------\n");
    printf("Program header details:\n");
    printf("------------------------------------------------\n");

    fseek(fp,ehdr.e_phoff,SEEK_SET);
    for(int i=0;i<ehdr.e_phnum;i++)
    {
        Elf32_Phdr phdr;

        if(fread(&phdr,ehdr.e_phentsize,1,fp)!=1)
            break;
        printf("--------------------\n");
        printf("pgmheader-->%d     |\n",i+1);
        printf("--------------------\n");
        printf("Type=%u\n",phdr.p_type);
        printf("offset=0x%x\n",phdr.p_offset);
        printf("virtual address=0x%x\n",phdr.p_vaddr);
        printf("file size=%u\n",phdr.p_filesz);
        printf("memory size=%u\n",phdr.p_memsz);
        printf("\n");
        


    }

    char ch1;
    printf("\n do you want to print section header:(y/n)");
    scanf(" %c",&ch1);
    if(ch1=='n' || ch1=='N') return 0;

    printf("-------------------------------\n");
    printf("Section header details:\n");
    printf("-------------------------------\n");
    fseek(fp,ehdr.e_shoff,SEEK_SET);

    for(int i=0;i<ehdr.e_shnum;i++)
    {
        Elf32_Shdr shdr;
        if(fread(&shdr,ehdr.e_shentsize,1,fp)!=1)
            break;
        printf("-------------------------\n");
        printf("section header-->%d    |\n",i+1);
        printf("-------------------------\n");
        printf("type=%u\n",shdr.sh_type);
        printf("offset=0x%x\n",shdr.sh_offset);
        printf("size=%u\n",shdr.sh_size);
        printf("address=0x%u\n",shdr.sh_addr);
    }

}

    

    fclose(fp);
    return 0;
}