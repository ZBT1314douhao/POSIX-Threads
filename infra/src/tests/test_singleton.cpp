
#include "gtest/gtest.h"

#include "os/thread.h"
#include "utils/singleton.h"
#include "utils/colorfulprint.h"

class CSingletonTest : public ::testing::Test
{
    // INFRA_SINGLETON_DECLARE(CSingletonTest)
    // 在这个测试用例中，必须声明为public

    INFRA_SINGLETON_DEFAULT_DECLARE(CSingletonTest)
public:
  CSingletonTest()
  {
      tracef("ctor called\n");
  }
  ~CSingletonTest()
  {
      tracef("dctor called\n");
  }
  const std::string& name() const
  {
      return m_singleton_name;
  }
  void setName(const std::string& new_name)
  {
      m_singleton_name = new_name;
  }

private:   

    // 如果上面的构造、析构函数不能满足测试要求，可以添加下面两个函数
    void SetUp() override
    {

    }
    void TearDown() override
    {

    }
    void TestBody()
    {

    }

private:
    std::string m_singleton_name;

};


// INFRA_SINGLETON_IMPLEMENT(CSingletonTest)

void func()
{
    std::string name = Infra::CSingletonDefault<CSingletonTest>::instance().name();
    tracef("in func, name = %s\n", name.c_str());
    Infra::CSingletonDefault<CSingletonTest>::instance().setName("new singleton");
}

TEST_F(CSingletonTest, SingletonCreate)
{
    CSingletonTest &s1 = Infra::CSingletonDefault<CSingletonTest>::instance();
    CSingletonTest &s2 = Infra::CSingletonDefault<CSingletonTest>::instance();
    EXPECT_EQ(&s1, &s2);
    s1.setName("singleton s1");
    Infra::CThreadLite tl(func);
    tl.create();
    tl.join();
    tracef("s1 = %p, s2 = %p\n", &s1, &s2);
    tracef("s1.name = %s, s2.name = %s\n", s1.name().c_str(), s2.name().c_str());
}
