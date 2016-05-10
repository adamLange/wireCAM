TEST(AlphaModulator,modulate)
{
  Slice slice;
  double alpha0(0);
  double alpha1(M_PI/4.0);

  std::vector<double> alpha0_arr = {-M_PI,-3*M_PI/4.0,-M_PI/2.0,
    -M_PI/4.0,0,M_PI/4.0,M_PI/2.0,3*M_PI/4.0,M_PI};

  //std::vector<double> delta_arr = {-M_PI,-3*M_PI/4.0,-M_PI/2.0,-M_PI/4.0
   // ,0,M_PI/4.0,M_PI/2.0,3*M_PI/4.0,M_PI};
  std::vector<double> delta_arr = {-3*M_PI/4.0,-M_PI/2.0,-M_PI/4.0
    ,0,M_PI/4.0,M_PI/2.0,3*M_PI/4.0};
  std::vector<double>  mod = {-2,-1,0,1,2};
  double delta, delta_mod, alpha1_mod;
  for (std::vector<double>::iterator alpha0_i = alpha0_arr.begin();
    alpha0_i != alpha0_arr.end(); ++alpha0_i)
  {
    for (std::vector<double>::iterator delta_i = delta_arr.begin();
      delta_i != delta_arr.end(); ++delta_i)
    {
      for (std::vector<double>::iterator mod_i = mod.begin();
        mod_i != mod.end(); ++mod_i)
      {
        alpha0 = *alpha0_i;
        delta = *delta_i;
        delta_mod =  delta + *mod_i*2*M_PI;
        alpha1 = alpha0 + delta;
        alpha1_mod = alpha0 + delta_mod;
        EXPECT_LT(delta,M_PI);
        EXPECT_TRUE(
          abs(slice.alphaModulator(alpha0,alpha1_mod)-alpha1)<1e-6)
          <<"  alpha0: "<<alpha0<<std::endl
          <<"  alpha1: "<<alpha1<<std::endl
          <<"  alpha1_mod: "<<alpha1_mod<<std::endl
          <<"  alphaModulator:"<<slice.alphaModulator(alpha0,alpha1_mod)
          <<std::endl;
      }
    }
  }
}
