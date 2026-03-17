#include <prob.H>

// Include the header for AMReX's parameter parsing
#include <AMReX_ParmParse.H>

void
amrex_probinit(
  const amrex_real* /*problo*/,
  const amrex_real* /*probhi*/)
{
  // --- Create the Embedded Boundary geometry ---

  // Create a ParmParse object with the prefix "prob"
  amrex::ParmParse pp("prob");

  // Define default values
  amrex::Real ramp_angle_deg = 0.0;
  amrex::RealArray ramp_start_point = {0.0, 0.0, 0.0};

  // Query the inputs file for the parameters. If they exist, the
  // variables will be updated. Otherwise, they keep the default values.
  pp.query("ramp_angle", ramp_angle_deg);
  pp.queryarr("ramp_start_point", ramp_start_point);

  // Now, perform the mathematical calculations in C++
  amrex::Print() << "Setting up a ramp with angle: " << ramp_angle_deg << " degrees.\n";
  const amrex::Real ramp_angle_rad = ramp_angle_deg * M_PI / 180.0;

  amrex::RealArray ramp_normal;
  ramp_normal.resize(AMREX_SPACEDIM);
  ramp_normal[0] =  std::sin(ramp_angle_rad);
  ramp_normal[1] = -std::cos(ramp_angle_rad);
#if (AMREX_SPACEDIM == 3)
  ramp_normal[2] =  0.0;
#endif

  // Create the plane implicit function using the calculated normal
  amrex::PlaneIF ramp(ramp_normal, ramp_start_point, false); // false = fluid is on the positive side of the normal

  // Build the geometry from the implicit function
  auto gshop = amrex::GeometryShop(ramp);
  auto& eb2_geom = amrex::EB2::IndexSpace::top().getEBGeom();

  // The final build step
  amrex::EB2::Build(gshop, eb2_geom, 0, 0);
}

