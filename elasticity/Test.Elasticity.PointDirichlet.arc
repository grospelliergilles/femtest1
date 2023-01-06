<?xml version="1.0"?>
<case codename="Elasticity" xml:lang="en" codeversion="1.0">
  <arcane>
    <title>Sample</title>
    <timeloop>ElasticityLoop</timeloop>
  </arcane>

  <arcane-post-processing>
   <output-period>1</output-period>
   <output>
     <variable>U</variable>
   </output>
  </arcane-post-processing>

  <meshes>
    <mesh>
      <filename>bar.msh</filename>
    </mesh>
  </meshes>

  <fem>
    <E>21.0e5</E>
    <nu>0.28</nu>
    <fy>-1.0</fy>
    <dirichlet-boundary-condition>
      <surface>left</surface>
      <u1>0.0</u1>
    </dirichlet-boundary-condition>
    <dirichlet-boundary-condition>
      <surface>right</surface>
      <u1>1.0</u1>
    </dirichlet-boundary-condition>
    <dirichlet-point-condition>
      <node>botLeft</node>
      <u1>0.0</u1>
      <u2>0.0</u2>
    </dirichlet-point-condition>
    <dirichlet-point-condition>
      <node>botRight</node>
      <u2>0.0</u2>
    </dirichlet-point-condition>
  </fem>
</case>
