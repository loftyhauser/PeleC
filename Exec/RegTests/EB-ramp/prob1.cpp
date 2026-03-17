// In the amrex_probinit function in prob.cpp

// Get parameters from the inputs file if needed
amrex::ParmParse pp("prob");
amrex::Real ramp_angle_deg = 15.0;
pp.query("ramp_angle", ramp_angle_deg); // Allow override from inputs file

// Define the geometry using C++ logic
amrex::Real ramp_angle_rad = ramp_angle_deg * M_PI / 180.0;
amrex::RealArray normal;
normal[0] = std::sin(ramp_angle_rad);
normal[1] = -std::cos(ramp_angle_rad);
normal[2] = 0.0;

amrex::RealArray point = {0.5, 0.0, 0.0};

// Create the implicit function object
amrex::PlaneIF ramp(normal, point);

// Build the geometry from the implicit function
amrex::GeometryShop gshop(ramp, 0, false);
amrex::EB2::Build(gshop, geom, 0, 0);

