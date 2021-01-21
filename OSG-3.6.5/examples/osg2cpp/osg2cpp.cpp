#include <osg/ArgumentParser>
#include <osg/ApplicationUsage>

#include <osgDB/ReadFile>
#include <osgDB/FileNameUtils>
#include <osgDB/fstream>

#include <iostream>

// Search in str for all occurrences of spat and replace them with rpat.
void searchAndReplace(std::string& str, const std::string& spat, const std::string& rpat)
{
    std::string::size_type pos = 0;
    while ((pos = str.find(spat, pos)) != std::string::npos)
    {
        str.replace(pos, spat.length(), rpat);
        pos += rpat.length();
    }
}

void writeShader(osg::Shader* shader, const std::string& cppFileName, const std::string& variableName)
{
    osgDB::ofstream fout(cppFileName.c_str());
    if (!fout)
    {
        std::cout<<"Error: could not open file `"<<cppFileName<<"` for writing."<<std::endl;
    }

    std::string shaderSource = shader->getShaderSource();
    searchAndReplace(shaderSource, "\r\n", "\n");
    searchAndReplace(shaderSource, "\r", "\n");
    searchAndReplace(shaderSource, "\"", "\\\"");

    std::string variableString = std::string("char ")+variableName+std::string("[] = ");

    std::string::size_type startOfLine = 0;
    std::string::size_type endOfLine = shaderSource.find_first_of('\n', startOfLine);

    if (endOfLine==std::string::npos)
    {
        fout<<variableString<<shaderSource<<"\\n\";"<<std::endl;
    }
    else
    {
        std::string padding(variableString.size(),' ');

        fout<<variableString<<"\""<<shaderSource.substr(startOfLine,endOfLine-startOfLine)<<"\\n\""<<std::endl;
        startOfLine = endOfLine+1;
        endOfLine = shaderSource.find_first_of('\n', startOfLine);

        while (endOfLine != std::string::npos)
        {
            fout<<padding<<"\""<<shaderSource.substr(startOfLine,endOfLine-startOfLine)<<"\\n\""<<std::endl;
            startOfLine = endOfLine + 1;
            endOfLine = shaderSource.find_first_of('\n', startOfLine);
        }
        fout<<padding<<"\""<<shaderSource.substr(startOfLine,endOfLine-startOfLine)<<"\\n\";"<<std::endl;
    }
    std::cout<<"Written shader to `"<<cppFileName<<"`"<<std::endl;
}

int main( int argc, char **argv )
{
    // use an ArgumentParser object to manage the program arguments.
    osg::ArgumentParser arguments(&argc,argv);

    // set up the usage document, in case we need to print out how to use this program.
    arguments.getApplicationUsage()->setApplicationName(arguments.getApplicationName());
    arguments.getApplicationUsage()->setDescription(arguments.getApplicationName()+" is a utility for converting glsl shader files into char arrays that can be compiled into applications.");
    arguments.getApplicationUsage()->setCommandLineUsage(arguments.getApplicationName()+" [options] filename ...");
    arguments.getApplicationUsage()->addCommandLineOption("--shader <filename>","Shader file to create a .cpp file for.");
    arguments.getApplicationUsage()->addCommandLineOption("--write-to-source-file-directory","Use the path to the source filename as the directory to write to.");
    arguments.getApplicationUsage()->addCommandLineOption("-h or --help","Display command line parameters");

    // if user request help write it out to cout.
    if (arguments.read("-h") || arguments.read("--help"))
    {
        arguments.getApplicationUsage()->write(std::cout);
        return 1;
    }

    bool useSamePathAsSourceFile = false;
    if (arguments.read("--write-to-source-file-directory")) useSamePathAsSourceFile = true;

    std::string filename;
    if (arguments.read("--shader",filename))
    {
        osg::ref_ptr<osg::Shader> shader = osgDB::readRefShaderFile(filename);
        if (shader.valid())
        {
            std::string name = osgDB::getStrippedName(filename);
            std::string path = osgDB::getFilePath(filename);
            std::string invalidCharacters = "-+/\\*=(){}[]:;<>,.?@'~#`!\"";
            std::string numbericCharacters = "0123456789";
            std::string::size_type pos = name.find_first_of(invalidCharacters);
            while (pos != std::string::npos)
            {
                name[pos] = '_';
                pos = name.find_first_of(invalidCharacters);
            }

            std::string ext = osgDB::getFileExtension(filename);
            std::string cppFileName = name + "_" + ext + ".cpp";
            if (useSamePathAsSourceFile) cppFileName = osgDB::concatPaths(path, cppFileName);

            std::string variableName = name + "_" + ext;
            writeShader(shader.get(), cppFileName, variableName);

            return 0;
        }
        else
        {
            std::cout<<"Error: could not find file '"<<filename<<"'"<<std::endl;
            return 1;
        }

    }

    std::cout<<"No appropriate command line options used."<<std::endl;

    arguments.getApplicationUsage()->write(std::cout);
    return 1;
}
