# SummerRutgers15

Jennifer Coulter 

Summer 2015 Work

Rutgers University 

Macros and scripts used in the calculation of thrust event shape variables. 

      //THRUST MINOR
      /*
//	Thoughts on best way to find an orthogonal vecotor. Based on this: http://math.stackexchange.com/questions/137362/how-to-find-perpendicular-vector-to-another-vector
//	get two vectors each orthogonal to original vector, each will have one component =0. then linear combos of these vectors as new thrust minor axes. I think you can assume that thrust will vary in a continuous fashion as we rotate the vector so quesiton becomes how to find maximum,optimization problem!!. Once this is figured out it shouldn't be too hard to apply to 3D case.

//	I think that coordinate cycling is a good solution here. see: 
//	http://www.google.com/url?sa=t&rct=j&q=&esrc=s&source=web&cd=3&ved=0CDAQFjACahUKEwilyPfVzfXGAhUL0IAKHdxIDF4&url=http%3A%2F%2Fmathforcollege.com%2Fnm%2Fmws%2Fgen%2F09opt%2Fmws_gen_opt_txt_multidirect.doc&ei=RSazVeWRLouggwTckbHwBQ&usg=AFQjCNEVHne0jfLEnbKvcuFuITc_BPjufw&sig2=7JDSCN8a-jh2tSk1eOjQqQ&bvm=bv.98717601,d.eXY
//	i found above as third link entitled: Multidimensional Direct Search Method - Math For College when i googled: two dimensional golder section search

      */