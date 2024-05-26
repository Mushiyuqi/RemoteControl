#pragma once
#include <cstring>
// 整个包的长度是 HEAD_LENGTH + MAX_LENGTH
#define MAX_LENGTH 1024 * 1024 //数据的最大长度
#define HEAD_LENGTH sizeof (size_t) //头部的长度

class MsgNode {
    friend class CSession;
    friend class CSessionThread;

public:
    // 为记录长度预留了空间的node
    // 用于发送一整个完整的包
    MsgNode(char * msg, std::size_t max_len): m_total_len(max_len +  HEAD_LENGTH), m_cur_len(0){
        m_data = new char[m_total_len+1]();
        memcpy(m_data, &max_len, HEAD_LENGTH);// 将消息长度写入m_data
        memcpy(m_data+ HEAD_LENGTH, msg, max_len);// 将消息内容写入m_data
        m_data[m_total_len] = '\0';// 给m_data添加结束符
    }
    // 什么都没做的node(创建空的node)
    MsgNode(std::size_t max_len)
        : m_total_len(max_len)
        , m_cur_len(0)
    {
        m_data = new char[m_total_len +1]();
    }
    ~MsgNode() {
        delete[] m_data;
    }

    // 重置 m_data 就不用再去开辟 m_data 了
    void Clear() {
        ::memset(m_data, 0, m_total_len);
        m_cur_len = 0;
    }
private:
    std::size_t m_cur_len;
    std::size_t m_total_len;
    char* m_data;
};
