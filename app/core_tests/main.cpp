#include <QTest>

#include "../../core_tests/src/tst_character.cpp"
#include "../../core_tests/src/tst_utils.cpp"

int main(int argc, char *argv[]) {
  utils_tests test1;
  QTest::qExec(&test1);

  character_tests test2;
  QTest::qExec(&test2);

  return 0;
}
