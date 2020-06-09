#include <iostream>
#include <string.h>
#include <unordered_map>
#include <dirent.h>
#include <sys/types.h>

using namespace std;

#define EI_NIDENT 16
  
#define EI_MAG0 0
#define EI_MAG1 1
#define EI_MAG2 2
#define EI_MAG3 3
#define EI_CLASS 4
#define EI_DATA  5

static const uint16_t ET_DYN = 3; /*object file type for shared object file*/

static const uint8_t ELFCLASSNONE = 0;/*Invalid class*/
static const uint8_t ELFCLASS32 = 1;/*32-bit objects*/
static const uint8_t ELFCLASS64 = 2;/*64-bit objects*/

static const uint8_t ELFDATAMSB= 2; /*big-endian*/
static const uint8_t ELFDATALSB= 1; /*little-endian*/
static const uint8_t ELFDATANONE= 0;

static const uint16_t EM_386 = 3;
static const uint16_t EM_X86_64 = 62;

static const uint16_t EM_QDSP6 = 164 ;
static const uint16_t EM_AARCH64 = 183;
static const uint16_t EM_ARM = 40;

static const uint16_t EM_MIPS = 8;
static const uint16_t EM_MIPS_RS3_LE = 10;

static const string currentdir = ".";
static const string parentdir = "..";

unordered_map <string,string> libarc;

 struct ElfHdr
{
  uint8_t     e_ident[EI_NIDENT];  /* Magic number and other info */
  uint16_t    e_type;              /* Object file type */
  uint16_t    e_machine;           /* Architecture */
} ;

void changeToLittleEndian( uint16_t &val , uint8_t encoding )
{
	if(encoding == ELFDATAMSB){
		uint16_t temp= val;
		val = 0;
		val = (temp>>8) | (temp<<8);
	}
}
bool isLibrary(const string elfFile) 
{
  struct ElfHdr header;
  bool ret =false;
  FILE* file = fopen(elfFile.c_str(), "rb");
  if(file) {
    fread(&header, 1, sizeof(header), file);

    if(header.e_ident[EI_MAG0] == 0x7f &&
       header.e_ident[EI_MAG1] == 'E' &&
       header.e_ident[EI_MAG2] == 'L' &&
       header.e_ident[EI_MAG3] == 'F' &&
	   header.e_ident[EI_CLASS] !=  ELFCLASSNONE &&
	   header.e_ident[EI_DATA] != ELFDATANONE )
	{
		changeToLittleEndian(header.e_type,header.e_ident[EI_DATA]);

		if(header.e_type==ET_DYN)
		{
			changeToLittleEndian(header.e_machine,header.e_ident[EI_DATA]);
			switch(header.e_machine)
			{
				case EM_386:
					ret=true;
					libarc[string(elfFile)]="x86";
					break;
				case EM_MIPS:
				case EM_MIPS_RS3_LE:
					ret=true;
					libarc[string(elfFile)]="mips";
					break;
				case EM_ARM:
					ret=true;
					libarc[string(elfFile)]="armeabi-v7a";
					break;
				case EM_X86_64:
					ret=true;
					libarc[string(elfFile)]="x86_64";
					break;
				case EM_AARCH64:
					ret=true;
					libarc[string(elfFile)]="arm64-v8a";
					break;
				case EM_QDSP6:
					ret=true;
					libarc[string(elfFile)]="armeabi";
					break;					
			}
		}	       
    }
    fclose(file);
  }
  return ret;
}


int findLibsAtInputPath(const string path)
{
   int count =0;
   struct dirent *entry;
   DIR *dir = opendir(path.c_str());
   
   if (dir == NULL) {

      return count;
   }
   while ((entry = readdir(dir)) != NULL) 
   {
           if(entry->d_type==DT_REG){
	   			count+=isLibrary(path+"/"+entry->d_name);
           }
	   	   if(entry->d_type==DT_DIR ){
	   	   		if(!(currentdir.compare(string(entry->d_name))==0 || parentdir.compare(string(entry->d_name))==0)){
	   	   			count+=findLibsAtInputPath(path+"/"+string(entry->d_name));
	   	   		}
	   	   }
   }
   closedir(dir);
   return count;
}

int main() {
	
	cout<<"Enter the Directory path"<<endl;
	string dir;
	cin>>dir;

   int nlibs = findLibsAtInputPath(dir.c_str());
   
   cout<<"Total number of libraries at path:\t"<< dir << "\t" <<nlibs<<endl;
   if (nlibs>0)
   {
	   cout<<"File			ArchType"<<endl;
	   cout<<"====          ============================="<<endl;
	   for(auto it=libarc.begin();it!=libarc.end();it++)
	   {
			cout<<it->first<<"\t"<<it->second<<endl;
	   }
	}
   return 0;
}