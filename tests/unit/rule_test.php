<?php
require_once('simpletest/autorun.php');
require_once('../Rule.class.inc');

class TestOfRules extends UnitTestCase
{
  function testRuleConstructor() {
    $rule = new TextFilters\Rule('/foo/m', 'bar');

    $this->assertEqual($rule->regex, '/foo/m');
    $this->assertEqual($rule->replacement, 'bar');
  }

  function testRuleSetProperties() {
    $rule = new TextFilters\Rule();
    $rule->regex = '/foo/m';
    $rule->replacement = 'bar';

    $this->assertEqual($rule->regex, '/foo/m');
    $this->assertEqual($rule->replacement, 'bar');
  }

  function testBadSetPropertyException() {
    $rule = new TextFilters\Rule();

    $this->expectException('InvalidArgumentException');
    $rule->foo = 'bar';
  }

  function testPreRuleConstructor() {
    $pre_rule = new TextFilters\Rule('/foo/m', 'bar');
    $rule = new TextFilters\Rule('/baz/m', 'qux', $pre_rule);

    $this->assertEqual($rule->pre_rule->regex, '/foo/m');
    $this->assertEqual($rule->pre_rule->replacement, 'bar');
    $this->assertEqual($rule->regex, '/baz/m');
    $this->assertEqual($rule->replacement, 'qux');
  }

  function testPreRuleSetProperties() {
    $rule = new TextFilters\Rule();
    $rule->pre_rule = new TextFilters\Rule('/foo/m', 'bar');
    $rule->regex = '/baz/m';
    $rule->replacement = 'qux';

    $this->assertEqual($rule->pre_rule->regex, '/foo/m');
    $this->assertEqual($rule->pre_rule->replacement, 'bar');
    $this->assertEqual($rule->regex, '/baz/m');
    $this->assertEqual($rule->replacement, 'qux');
  }

  function testPostRuleConstructor() {
    $post_rule = new TextFilters\Rule('/foo/m', 'bar');
    $rule = new TextFilters\Rule('/baz/m', 'qux', null, $post_rule);

    $this->assertEqual($rule->post_rule->regex, '/foo/m');
    $this->assertEqual($rule->post_rule->replacement, 'bar');
    $this->assertEqual($rule->regex, '/baz/m');
    $this->assertEqual($rule->replacement, 'qux');
  }

  function testPostRuleSetProperties() {
    $rule = new TextFilters\Rule();
    $rule->post_rule = new TextFilters\Rule('/foo/m', 'bar');
    $rule->regex = '/baz/m';
    $rule->replacement = 'qux';

    $this->assertEqual($rule->post_rule->regex, '/foo/m');
    $this->assertEqual($rule->post_rule->replacement, 'bar');
    $this->assertEqual($rule->regex, '/baz/m');
    $this->assertEqual($rule->replacement, 'qux');
  }

}
