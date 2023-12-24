<?xml version='1.0'?>
<case codename="Passmo" xml:lang="en" codeversion="1.0">
  <arcane>
    <title>Sample</title>
    <timeloop>PassmoLoop</timeloop>
  </arcane>
  <arcane-post-processing>
   <output-period>1</output-period>
   <output>
     <variable>Displ</variable>
     <variable>Vel</variable>
     <variable>Acc</variable>
   </output>
  </arcane-post-processing>

  <meshes>
    <mesh>
      <filename>sq4.msh</filename>
      <initialization>
        <variable><name>Rho</name><value>2500.0</value><group>surface</group></variable>
        <variable><name>Young</name><value>6.62e6</value><group>surface</group></variable>
        <variable><name>Nu</name><value>0.45</value><group>surface</group></variable>
      </initialization>
    </mesh>
  </meshes>

  <elastodynamic>
    <analysis-type>planestrain</analysis-type>
    <start>0.</start>
    <final-time>0.08</final-time>
    <deltat>0.01</deltat>
    <beta>0.25</beta>
    <gamma>0.5</gamma>
    <alfa_method>false</alfa_method>
    <enforce-Dirichlet-method>Penalty</enforce-Dirichlet-method>
    <penalty>1.e30</penalty>
    <linop-nstep>10</linop-nstep>
    <gz>10.0</gz>

    <paraxial-boundary-condition>
      <surface>bottom</surface>
    </paraxial-boundary-condition>

    <paraxial-boundary-condition>
      <surface>left</surface>
    </paraxial-boundary-condition>

    <paraxial-boundary-condition>
      <surface>right</surface>
    </paraxial-boundary-condition>

    <neumann-boundary-condition>
      <surface>top</surface>
      <curve>semi-circle-soil-traction.txt</curve>
    </neumann-boundary-condition>

    <linear-system>
      <solver-backend>petsc</solver-backend>
      <preconditioner>ilu</preconditioner>
    </linear-system>
  </elastodynamic>
</case>
