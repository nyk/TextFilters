<?php
require_once('simpletest/autorun.php');

class TextFiltersTestSuite extends TestSuite
{
  function __construct() {
    parent::__construct();
    $this->addFile('unit/rule_test.php');
    $this->addFile('unit/filter_rule_test.php');
    $this->addFile('unit/filter_test.php');
  }
}
