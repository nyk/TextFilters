<?php
require_once('simpletest/autorun.php');
require_once('../Rule.class.inc');

class TestOfFilterRules extends UnitTestCase
{

  function testWithTitleCaseFilter() {
    $pre_rule = new TextFilters\Rule('/\bdel\b/m', '@@@DEL@@@');
    $post_rule = new TextFilters\Rule('/@@@DEL@@@/mi', 'del');
    $rule = new TextFilters\Rule('filter', 'titleCase', $pre_rule, $post_rule);

    $ugly = 'sierra del fuego';
    $pretty = TextFilters\Filter::input($ugly)
                    ->rules(array($rule))
                    ->output();

    $this->assertEqual($pretty, 'Sierra del Fuego');
  }

  function testWithSentenceCaseFilter() {
    $pre_rule = new TextFilters\Rule('/Jr\./', '@@@JUNIOR@@@');
    $post_rule = new TextFilters\Rule('/@@@JUNIOR@@@/', 'Jr.');
    $rule = new TextFilters\Rule('filter', 'sentenceCase', $pre_rule, $post_rule);

    $ugly = 'we know that Louis Gossett, Jr. is a well known actor';
    $pretty = TextFilters\Filter::input($ugly)
                ->rules(array($rule))
                ->output();

    $this->assertEqual($pretty, 'We know that Louis Gossett, Jr. is a well known actor');
  }

  function testCityNamesFilter() {
    $this->assertEqual(TextFilters\Filter::input('saint marie sur la mer')->location()->output(), 'Saint Marie sur la Mer');
    $this->assertEqual(TextFilters\Filter::input('Dissen am teutoburger wald')->location()->output(), 'Dissen am Teutoburger Wald');
    $this->assertEqual(TextFilters\Filter::input('يعقشئد')->location()->output(), 'يعقشئد');
    $this->assertEqual(TextFilters\Filter::input('中西區')->location()->output(), '中西區');
    $this->assertEqual(TextFilters\Filter::input('Санкт-Петербург')->location()->output(), 'Санкт-Петербург');
    $this->assertEqual(TextFilters\Filter::input('Αθήνα')->location()->output(), 'Αθήνα');
    $this->assertEqual(TextFilters\Filter::input('αθήνα')->location()->output(), 'Αθήνα');
  }
}
