
#include "gtest/gtest.h"

#include "os/thread.h"
#include "os/thread_specific_data.h"
#include "utils/colorfulprint.h"

class CTSDTest : public ::testing::Test
{
public:
  CTSDTest()
  {
      tracef("ctor called\n");
  }
  ~CTSDTest()
  {
      tracef("dctor called\n");
  }
  const std::string& name() const
  {
      return m_name;
  }
  void setName(const std::string& new_name)
  {
      m_name = new_name;
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
    std::string m_name;

};

void tsd_func()
{

}

TEST_F(CTSDTest, TSDGet)
{
    using namespace Infra;
    CThreadSpecificData tsd;
    int value = 100;
    /// When doing pointer comparisons use *_EQ(ptr, nullptr) and *_NE(ptr, nullptr) 
    /// instead of *_EQ(ptr, NULL) and *_NE(ptr, NULL). 
    /// This is because nullptr is typed while NULL is not
    //EXPECT_EQ(tsd.get(), nullptr);
}

TEST_F(CTSDTest, TSDSet)
{
    using namespace Infra;
    CThreadSpecificData tsd;
    int value = 100;
    EXPECT_TRUE(tsd.set(&value));
}

TEST_F(CTSDTest, TSDSetAndGet)
{
    using namespace Infra;
    CThreadSpecificData tsd1;
    CThreadSpecificData tsd2;
    int value = 100;
    //EXPECT_EQ(tsd1.get(&value), false);
    //tracef("value = %d\n", value);
    value = 200;
    EXPECT_EQ(tsd1.set(&value), true);
    //EXPECT_NE(tsd1.get(), nullptr);
    // int val = *(int *)tsd1.get((void *)&value);
    // tracef("value = %d, val = %d(addr %p)\n", value, val, &val);

    CThreadLite tl(tsd_func);
    tl.create();
    tl.join();
}
