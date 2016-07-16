<?php

namespace TextFilters;

interface iRule
{
  public function preProcess(&$text);
  public function postProcess(&$text);
}
