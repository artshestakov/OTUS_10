#include "cmd.h"
#include "utils.h"
//-----------------------------------------------------------------------------
cmd::cmd(unsigned int block_count)
    : m_BlockCount(block_count),
    m_BlockDepth(0)
{
    if (block_count > 0)
    {
        SetBlockCount(block_count);
    }
}
//-----------------------------------------------------------------------------
cmd::~cmd()
{

}
//-----------------------------------------------------------------------------
void cmd::Subscribe(Observer* object)
{
    m_Subscribers.emplace_back(object);
}
//-----------------------------------------------------------------------------
const std::string& cmd::GetErrorString() const
{
    return m_ErrorString;
}
//-----------------------------------------------------------------------------
bool cmd::ParseArgument(int argc, char** argv)
{
    if (argc <= 1)
    {
        m_ErrorString = "You didn't specify an argument";
        return false;
    }

    std::string str = argv[1];
    int n = 0;

    try
    {
        n = std::stoi(str);
    }
    catch (const std::exception& e)
    {
        m_ErrorString = "Can't parse argument '" + str + "': " + std::string(e.what());
        return false;
    }

    if (n <= 0)
    {
        m_ErrorString = "Argument must be greater than zero";
        return false;
    }

    SetBlockCount((unsigned int)n);
    return true;
}
//-----------------------------------------------------------------------------
void cmd::ReadConsole()
{
    std::cout << "Enter the \"\\q\" command for exit." << std::endl << std::endl;

    std::string command;
    while (std::getline(std::cin, command))
    {
        ReadConsole(command);
    }
}
//-----------------------------------------------------------------------------
void cmd::ReadConsole(const std::string& command)
{
    auto vec = utils::split_string(command, '\n');
    for (const std::string& cm : vec)
    {
        if (cm == "{")
        {
            if (!m_Vector.empty() && m_BlockDepth == 0)
            {
                PrintAndClearVector();
            }

            ++m_BlockDepth;
            return; //Не допускаем лишней проверки внизу цикла
        }
        else if (cm == "}")
        {
            --m_BlockDepth;
            if (m_BlockDepth == 0)
            {
                PrintAndClearVector();
            }
            return; //Не допускаем лишней проверки внизу цикла
        }
        else if (m_BlockDepth > 0 && cm.empty())
        {
            PrintAndClearVector();
            m_BlockDepth = 0;
            return; //Не допускаем лишней проверки внизу цикла
        }
        else if (cm == "\\q") //Не забываем про корректный выход из программы
        {
            std::cout << "The program will be closed" << std::endl;
            break;
        }
        else //Считаем что пришла стандартная команда
        {
            AddCommand(cm);
        }

        if (m_BlockDepth == 0 && (m_Vector.size() == m_BlockCount || cm.empty()))
        {
            PrintAndClearVector();
        }
    }
}
//-----------------------------------------------------------------------------
void cmd::AddCommand(const std::string& command)
{
    if (!command.empty())
    {
        m_Vector.emplace_back(command);

        if (!m_Time.has_value())
        {
            m_Time = std::chrono::system_clock::now();
        }
    }
}
//-----------------------------------------------------------------------------
void cmd::PrintAndClearVector()
{
    std::stringstream str_stream;

    str_stream << "bulk: ";
    for (size_t i = 0, c = m_Vector.size(); i < c; ++i)
    {
        const std::string& s = m_Vector[i];
        str_stream << s;
        if (i != c - 1)
        {
            str_stream << ", ";
        }
    }
    str_stream << std::endl;

    Notify(str_stream.str());

    m_Vector.clear();
    m_Time = std::nullopt;
}
//-----------------------------------------------------------------------------
void cmd::Notify(const std::string& s)
{
    for (auto subscriber : m_Subscribers)
    {
        subscriber->Update(s, m_Time.value());
    }
}
//-----------------------------------------------------------------------------
void cmd::SetBlockCount(unsigned int block_count)
{
    m_BlockCount = block_count;
    m_Vector.reserve(m_BlockCount);
}
//-----------------------------------------------------------------------------
