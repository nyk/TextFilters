<?php

namespace TextFilters;

require_once('iRule.interface.inc');

class Rule implements iRule
{
  protected $regex, $replacement, $pre_rule, $post_rule;

  public function __construct($regex = null, $replacement = null, $pre_rule = null, $post_rule = null) {
    $this->regex = $regex;
    $this->replacement = $replacement;
    $this->pre_rule = $pre_rule;
    $this->post_rule = $post_rule;
  }

  public function __set($name, $value) {
    if (!in_array($name, array_keys(get_object_vars($this)))) {
       throw new \InvalidArgumentException('Property: "' . $name . '" is not a property of Rule objects');
    } else {
       $this->{$name} = $value;
    }
  }

  public function __get($name) {
    if (!in_array($name, array_keys(get_object_vars($this)))) {
      throw new \InvalidArgumentException('Property: "' . $name . '" is not a property of Rule objects');
    } else {
      return $this->{$name};
    }
  }

  public function preProcess(&$text) {
    $this->processRule($text);
  }

  public function postProcess(&$text) {
    $this->processRule($text, 'post_rule');
  }

  protected function processRule(&$text, $type = 'pre_rule') {
    $rule = $this->{$type};
    if (is_object($rule)) {
      $text = preg_replace($rule->regex, $rule->replacement, $text);
    }
  }
}
