#include <iostream>
#include <algorithm>
#include <math.h>
#include <bitset>
#include <string>
#include <map>
#include <fstream>

enum syntax{
    nothing = 0,
    reg,
    imm,
    immreg,
};
enum class dataJob{
    nothing = 0,
    writeDA,
    readDA,
};

int main(){
    std::string fname;
    std::cout << "give the path or name(without extension) to/of the file to convert: ";
    std::cin >> fname;
    unsigned int lineIndex = 0;
    std::string binary = "";
    std::string AsciiEncodeBinary = "";
    std::string line;
    std::ifstream myfile (fname+".txt");
    std::map<std::string, unsigned int> labels;
    if (myfile.is_open())
    {
        while ( std::getline(myfile,line) ){
            std::string word;
            bool isLabel= false;
            ++lineIndex;

            for (int i = 0;i < line.length();i++){
                if (line.at(i) == ';'){
                    --lineIndex;
                    break;
                }
                if (line.at(i) != ' ' && i+1 != line.length()){
                    word += line.at(i);
                    continue;
                }else{
                    if (word != "label" && !isLabel){
                        break;
                    }
                    if (isLabel){
                        word += line.at(i);
                        //std::cout << " label " << word << " on line " << lineIndex << "\n";
                        labels["#"+word] = lineIndex-1;
                    }
                    isLabel = true;
                    word.clear();
                }
            }
        }
        myfile.clear();
        myfile.seekg(0);
        lineIndex = 0;
        while ( std::getline (myfile,line) ){
            std::string word;
            unsigned int tmpBinary[64]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            //std::bitset<64> tmpBinary;
            unsigned int level = 0;
            unsigned int instructionLenght = 99;
            bool immAlr = false;
            bool comment = false;
            bool isLabel = false;
            syntax s[3];
            dataJob job[3];
            lineIndex++;

            for (int i = 0;i < line.length()+1;i++){
                if (i < line.length())
                    if (line.at(i) != ' '){
                        if (line.at(i) == ';'){
                            if (level == 0){
                                comment = true;
                                //lineIndex--;
                            }
                            if (level-1<instructionLenght && level > 0){
                                std::cout << "[error] | in line " << lineIndex << "\n";
                                std::cout <<       "\t| " << line << "\n";
                                std::cout <<       "\t| not enough arguments give required are " << instructionLenght << " arguments but given are " << level-1 << "\n";
                                return 1;
                            }
                            break;
                        }
                        word += line.at(i);//ADD CHAR TO WORD UNTIL A SPACE IS THERE
                        continue;
                    }
                if (level == 0){
                    if (word == "mv"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 0;
                        tmpBinary[62] = 0;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 0;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = reg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::writeDA;
                        
                        instructionLenght = 2;
                    }else if (word == "push"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 0;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 0;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "pop"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 0;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 0;
                        s[0] = reg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::writeDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmp"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 0;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpC"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpSL"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 1;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpUL"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 1;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpEQ"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        tmpBinary[58] = 1;
                        tmpBinary[57] = 1;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpSG"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 1;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpUG"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 1;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 1;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "jmpIF"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 1;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 1;
                        tmpBinary[56] = 1;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        
                        instructionLenght = 1;
                    }else if (word == "call"){
                        //tmpBinary index 63 to 55 is zero
                        //opcode
                        tmpBinary[63] = 0;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 1;// 4
                        //extra opcode
                        tmpBinary[60] = 0;// 8
                        tmpBinary[59] = 0;// 16
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 1;
                    }else if (word == "ret"){
                        //tmpBinary index 63 to 55 is zero
                        //opcode
                        tmpBinary[63] = 0;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 1;// 4
                        //extra opcode
                        tmpBinary[60] = 1;// 8
                        tmpBinary[59] = 0;// 16
                        //bit select
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = nothing;
                        s[1] = nothing;
                        s[2] = nothing;
                        job[0] = dataJob::nothing;
                        job[1] = dataJob::nothing;
                        
                        instructionLenght = 0;
                    }else if(word == "write"){
                        //opcode
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 0;
                        tmpBinary[61] = 1;
                        //extra opcode
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 0;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        job[2] = dataJob::nothing;

                        instructionLenght = 2;
                    }else if(word == "read"){
                        //opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 1;// 4
                        //extra opcode
                        tmpBinary[60] = 1;// 8
                        tmpBinary[59] = 0;// 16
                        tmpBinary[58] = 0;// 32
                        tmpBinary[57] = 0;// 64
                        tmpBinary[56] = 0;// 128
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 0;
                        s[0] = immreg;
                        s[1] = reg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::writeDA;
                        job[2] = dataJob::nothing;

                        instructionLenght = 2;
                    }else if(word == "store"){
                        //opcode
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 0;
                        tmpBinary[61] = 1;
                        //extra opcode
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 1;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 0;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::nothing;
                        job[2] = dataJob::nothing;

                        instructionLenght = 2;
                    }else if(word == "load"){
                        //opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 1;// 4
                        //extra opcode
                        tmpBinary[60] = 1;// 8 //2 8 //3 16
                        tmpBinary[59] = 1;// 16
                        tmpBinary[58] = 0;// 32
                        tmpBinary[57] = 0;// 64
                        tmpBinary[56] = 0;// 128
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 0;
                        tmpBinary[37] = 0;
                        s[0] = immreg;
                        s[1] = reg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::writeDA;
                        job[2] = dataJob::nothing;

                        instructionLenght = 2;
                    }else if(word == "add"){
                        //tmpBinary index 63 to 55 is zero
                        tmpBinary[63] = 1;
                        tmpBinary[62] = 0;
                        tmpBinary[61] = 0;
                        tmpBinary[60] = 0;
                        tmpBinary[59] = 0;
                        tmpBinary[58] = 0;
                        tmpBinary[57] = 1;
                        tmpBinary[56] = 0;
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "sub"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "mult"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "multl"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "div"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "mod"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;

                        instructionLenght = 3;
                    }else if(word == "neg"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = reg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::writeDA;

                        instructionLenght = 2;
                    }else if(word == "not"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 0;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = reg;
                        s[2] = nothing;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::writeDA;

                        instructionLenght = 2;
                    }else if(word == "rotl"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "rotr"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "or"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "and"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 0;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "nor"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "xor"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 1;// 1
                        tmpBinary[58] = 0;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "nand"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "xnor"){
                        //this is the opcode
                        tmpBinary[63] = 1;// 1
                        tmpBinary[62] = 0;// 2
                        tmpBinary[61] = 0;// 4
                        tmpBinary[60] = 0;// 8
                        //extra opcode
                        tmpBinary[59] = 0;// 1
                        tmpBinary[58] = 1;// 2
                        tmpBinary[57] = 1;// 4
                        tmpBinary[56] = 1;// 8
                        //36 and 37 is what data line goes to the save line for the register
                        tmpBinary[36] = 1;
                        tmpBinary[37] = 1;
                        s[0] = immreg;
                        s[1] = immreg;
                        s[2] = reg;
                        job[0] = dataJob::readDA;
                        job[1] = dataJob::readDA;
                        job[2] = dataJob::writeDA;
                        instructionLenght = 3;
                    }else if(word == "label"){
                        isLabel = true;
                        instructionLenght = 1;
                        comment = true;
                        lineIndex--;
                    }else if(word == ""){
                        comment = true;
                        //lineIndex--;
                        //std::cout << "free in " << lineIndex;
                        break;
                    }else{
                        std::cout << "[error] | on line " << lineIndex << "\n\t| opcode " << word << " does not exist" << std::endl;
                        return 1;
                    }
                    
                }else{
                   if (isLabel){
                        //labels["#"+word] = lineIndex-1;
                        
                        break;
                    }
                    if (labels.find(word) != labels.end()){
                        std::string idx = word;
                        //std::cout << word << " : " << labels[word] << std::endl;
                        word = std::to_string(labels.at(idx));
                        word.insert(0,1,'#');//insert in word at position 0 1 character '#'
                    }

                    if (word[0] == '#'){
                        if (immAlr){
                            std::cout << "[error] | in line " << lineIndex << "\n\t| " << line << "\n\t| given two immediate numbers while only one is possible\n";
                            return 1;
                        }
                        bool isInteger = true;
                        std::string wordd = word;
                        wordd.erase(std::remove(wordd.begin(),wordd.end(),'#'),wordd.end());
                        //std::cout << wordd << "\n";
                        for (const char ch : wordd){
                            if (std::isdigit(ch) == false){
                                isInteger = false;
                            }
                        }
                        if (isInteger){
                            if (s[level-1] ==  imm || s[level-1] ==  immreg){
                                immAlr = true;
                                if (level == 1){
                                    tmpBinary[43] = 1;
                                }else if (level == 2){
                                    tmpBinary[41] = 1;
                                }else{
                                    tmpBinary[39] = 1;
                                }
                                unsigned int offset = 0;
                                unsigned int num = std::stoi(wordd);
                                do{
                                    tmpBinary[31-offset] = num%2;
                                    num /=  2;
                                    offset++;
                                }while(num != 0);
                            }else{
                                std::cout << "error in line:" << lineIndex << " at word:" << word << " it schould be an integer\n";
                                return 1;
                            }
                        }else {
                            int extra = 0;
                            if (word.length() > 5){
                                std::cout << "[error] | " << "on line " << lineIndex << "\n";
                                std::cout <<       "\t| " << line << "\n";
                                std::cout <<       "\t| cant convert ascii string longer than 4 characters\n";
                                return 1;
                            }
                            if (word.length() == 1){
                                std::cout << "[error] | " << "on line " << lineIndex << "\n";
                                std::cout <<       "\t| " << line << "\n";
                                std::cout <<       "\t| no value given after '#'\n";
                                return 1;
                            }
                            if (level == 1){
                                tmpBinary[43] = 1;
                            }else if (level == 2){
                                tmpBinary[41] = 1;
                            }else{
                                tmpBinary[39] = 1;
                            }
                            //std::cout << "word " << word << "\n";
                            int byte = 0;
                            for (char ascii : word){
                                if (ascii == '#')
                                    continue;
                                unsigned int n = int(ascii);
                                if (ascii == '|')
                                    n = int(' ');
                                int offset = 0;
                                std::string b;
                                //std::cout << "\t" << ascii << "\n";
                                while (n != 0){
                                    tmpBinary[31-(byte*8)-offset] = n%2;
                                    //std::cout << "\t\t" << n%2 << "\n";
                                    b += (n%2 == 1) ? "1" : "0";
                                    n /= 2;
                                    offset++;
                                }
                                //std::cout << "\t => " << ascii << " is " << int(ascii) << " in binary -> " << b << "\n";
                                byte++;
                            }
                        }
                    }else{
                        if(s[level-1] == reg || s[level-1] ==  immreg){
                            if (job[level-1] == dataJob::writeDA){
                                if (level == 1){
                                    tmpBinary[43] = 1;
                                    tmpBinary[42] = 1;
                                }else if (level == 2){
                                    tmpBinary[41] = 1;//41
                                    tmpBinary[40] = 1;
                                }else{
                                    tmpBinary[39] = 1;
                                    tmpBinary[38] = 1;
                                }
                            }else if(job[level-1] == dataJob::readDA){
                                if (level == 1){
                                    tmpBinary[43] = 0;
                                    tmpBinary[42] = 1;
                                }else if (level == 2){
                                    tmpBinary[41] = 0;//41
                                    tmpBinary[40] = 1;
                                }else{
                                    tmpBinary[39] = 0;
                                    tmpBinary[38] = 1;
                                }
                            }
                            if (word == "RM1"){
                                if (level == 1){
                                    tmpBinary[55] = 0;
                                    tmpBinary[54] = 0;
                                    tmpBinary[53] = 0;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 0;
                                    tmpBinary[50] = 0;
                                    tmpBinary[49] = 0;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 0;
                                    tmpBinary[46] = 0;
                                    tmpBinary[45] = 0;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RM2"){
                                if (level == 1){
                                    tmpBinary[55] = 1;
                                    tmpBinary[54] = 0;
                                    tmpBinary[53] = 0;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 1;
                                    tmpBinary[50] = 0;
                                    tmpBinary[49] = 0;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 1;
                                    tmpBinary[46] = 0;
                                    tmpBinary[45] = 0;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RGP1"){
                                if (level == 1){
                                    tmpBinary[55] = 0;
                                    tmpBinary[54] = 1;
                                    tmpBinary[53] = 0;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 0;
                                    tmpBinary[50] = 1;
                                    tmpBinary[49] = 0;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 0;
                                    tmpBinary[46] = 1;
                                    tmpBinary[45] = 0;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RGP2"){
                                if (level == 1){
                                    tmpBinary[55] = 1;
                                    tmpBinary[54] = 1;
                                    tmpBinary[53] = 0;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 1;
                                    tmpBinary[50] = 1;
                                    tmpBinary[49] = 0;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 1;
                                    tmpBinary[46] = 1;
                                    tmpBinary[45] = 0;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RBC1"){
                                if (level == 1){
                                    tmpBinary[55] = 0;
                                    tmpBinary[54] = 0;
                                    tmpBinary[53] = 1;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 0;
                                    tmpBinary[50] = 0;
                                    tmpBinary[49] = 1;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 0;
                                    tmpBinary[46] = 0;
                                    tmpBinary[45] = 1;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RBC2"){
                                if (level == 1){
                                    tmpBinary[55] = 1;
                                    tmpBinary[54] = 0;
                                    tmpBinary[53] = 1;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 1;
                                    tmpBinary[50] = 0;
                                    tmpBinary[49] = 1;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 1;
                                    tmpBinary[46] = 0;
                                    tmpBinary[45] = 1;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RTV"){
                                if (level == 1){
                                    tmpBinary[55] = 0;
                                    tmpBinary[54] = 1;
                                    tmpBinary[53] = 1;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 0;
                                    tmpBinary[50] = 1;
                                    tmpBinary[49] = 1;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 0;
                                    tmpBinary[46] = 1;
                                    tmpBinary[45] = 1;
                                    tmpBinary[44] = 0;
                                }
                            }else
                            if (word == "RAF"){
                                if (level == 1){
                                    tmpBinary[55] = 1;
                                    tmpBinary[54] = 1;
                                    tmpBinary[53] = 1;
                                    tmpBinary[52] = 0;
                                }else if (level == 2){
                                    tmpBinary[51] = 1;
                                    tmpBinary[50] = 1;
                                    tmpBinary[49] = 1;
                                    tmpBinary[48] = 0;
                                }else{
                                    tmpBinary[47] = 1;
                                    tmpBinary[46] = 1;
                                    tmpBinary[45] = 1;
                                    tmpBinary[44] = 0;
                                }
                            }else if (word == "CLK"){
                                    tmpBinary[55-((level-1)*4)] = 0;
                                    tmpBinary[54-((level-1)*4)] = 0;
                                    tmpBinary[53-((level-1)*4)] = 0;
                                    tmpBinary[52-((level-1)*4)] = 1;
                            }else if (word == "out1"){
                                    tmpBinary[55-((level-1)*4)] = 1;
                                    tmpBinary[54-((level-1)*4)] = 0;
                                    tmpBinary[53-((level-1)*4)] = 0;
                                    tmpBinary[52-((level-1)*4)] = 1;
                            }else if (word == "out2"){
                                    tmpBinary[55-((level-1)*4)] = 0;
                                    tmpBinary[54-((level-1)*4)] = 1;
                                    tmpBinary[53-((level-1)*4)] = 0;
                                    tmpBinary[52-((level-1)*4)] = 1;
                            }else{
                                std::cout << "[error] | on line: " << lineIndex << "\n\t| " << line << "\n\t| register " << word << " does not exist" << "\n\t| if you mean a immideate number put '#' before it" << std::endl;
                                return 1;
                            }

                        }
                        else{
                            std::cout << "error in line: " << lineIndex << " at word: " << word << " it schould be a reg\n";
                            return 1;
                        }
                    }
                }
                if (i >= line.length() && level != instructionLenght){
                    std::cout << "error on line: " << lineIndex << " needed are " << instructionLenght << " arguments but only " << level << " instructions have been given\n";
                    return 1; 
                }
                level++;
                word.clear();
            }
            //std::reverse(tmpBinary.begin(),tmpBinary.end());
            if (!comment){
                std::string o;
                int index = 0;
                int num = 0;
                for (int st : tmpBinary){
                    o += (st == 1) ? "1" : "0";
                    if (st == 1){
                        num += pow(2,7-index);
                    }
                    //std::cout << 2 << " to the power of " << 7-index << " is " << pow(2,7-index) << std::endl;
                    index++;
                    if (index==8){
                        o += " ";
                        index = 0;
                        AsciiEncodeBinary += char(num+1);
                        num = 0;
                    }
                }
                binary += o;
            }
        }
        myfile.close();
    }else{
        std::cout << "Unable to open file " << fname + ".txt\n";
        return 1;
    }
    //for (const auto&[name,line] : labels)
    //    std::cout << name << " : " << line << std::endl;


    std::ofstream bin;
    bin.open (fname+"-bin.txt");
    bin << binary;
    bin.close();
    std::ofstream asciiEnc;
    asciiEnc.open (fname+"-ascii.txt");
    asciiEnc << AsciiEncodeBinary;
    asciiEnc.close();

    return 0;
}