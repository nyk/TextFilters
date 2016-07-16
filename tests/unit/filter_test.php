<?php
require_once('simpletest/autorun.php');
require_once('../Filter.class.inc');

class TestOfFilter extends UnitTestCase
{
  function testFilterConstructor() {
    $filter = new TextFilters\Filter('foo');

    $this->assertEqual($filter->output(), 'foo');
  }

  function testFilterFactory() {
    $filter = TextFilters\Filter::input('foo');

    $this->assertEqual($filter->output(), 'foo');
  }

  function testWhitespaceFilter() {
    $ugly = '   Foo is   not bar   ,  nor is  it  qux  ';

    $pretty = TextFilters\Filter::input($ugly)
              ->whitespace()
              ->output();

    $this->assertEqual($pretty, 'Foo is not bar , nor is it qux');
  }

  function testPunctuationFilter() {
    $ugly = '. This is foo., not bar.. baz is.. qux';

    $pretty = TextFilters\Filter::input($ugly)
                ->punctuation()
                ->output();

    $this->assertEqual($pretty, 'This is foo. not bar. baz is. qux.');
  }

  function testNoFullStopFilter() {
    $ugly = 'This is foo.';

    $pretty = TextFilters\Filter::input($ugly)
                ->noFullStop()
                ->output();

    $this->assertEqual($pretty, 'This is foo');
  }

  function testBracketsFilter() {
  }

  function testLowPercentUpFilter() {
    // 70% default
    $pretty = TextFilters\Filter::input('ABCDEFGhij')
                ->lowPercentUp()
                ->output();

    $this->assertEqual($pretty, 'abcdefghij');

    // Set percentage
    $pretty = TextFilters\Filter::input('ABCDEfghij')
                ->lowPercentUp(50)
                ->output();

    $this->assertEqual($pretty, 'abcdefghij');

    // Test less than percent threshold
    $pretty = TextFilters\Filter::input('ABCDEFghij')
                ->lowPercentUp()
                ->output();

    $this->assertEqual($pretty, 'ABCDEFghij'); // remains the same.
  }

  function testSentenceCaseFilter() {
  }

  function testTitleCaseFilter() {
  }

  function testReplaceRulesFilter() {
    $rules = array(
      new TextFilters\Rule('/this/m', '@@@THIS@@@'),
      new TextFilters\Rule('/that/m', '@@@THAT@@@'),
      new TextFilters\Rule('/@@@THIS@@@/m', 'that'),
      new TextFilters\Rule('/@@@THAT@@@/m', 'this'),
    );

    $ugly = 'Take that and make it this, then take this and make it that.';

    $pretty = TextFilters\Filter::input($ugly)
                ->rules($rules)
                ->output();

    $this->assertEqual($pretty, 'Take this and make it that, then take that and make it this.');
  }

}
