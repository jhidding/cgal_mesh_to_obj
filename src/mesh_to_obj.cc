#include <iostream>
#include <cstdlib>
#include <functional>
#include <map>

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>

using Triangulation = CGAL::Surface_mesh_default_triangulation_3;
using Mesh          = CGAL::Complex_2_in_triangulation_3<Triangulation>;
using Geometry      = Triangulation::Geom_traits;
using Sphere        = Geometry::Sphere_3;
using Point         = Geometry::Point_3;
using real_t        = Geometry::FT;
using Function      = std::function<real_t (Point)>;
using Surface       = CGAL::Implicit_surface_3<Geometry, Function>;

Function sphere_function(real_t radius)
{
  real_t radius_squared = radius * radius;
  return [=] (Point const &p) {
    auto v = p - CGAL::ORIGIN;
    return v*v - radius_squared;
  };
}

void write_to_obj(std::ostream &out, Mesh &mesh)
{
  std::map<Mesh::Vertex_handle, unsigned> vertex_map;
  unsigned count = 1;

  for (auto v  = mesh.vertices_begin();
            v != mesh.vertices_end();
            ++v) {
    vertex_map[v] = count;
    ++count;
    auto point = v->point();
    out << "v " << point << " 0.0\n";
  }

  out << "\n";

  for (auto f  = mesh.facets_begin();
            f != mesh.facets_end();
            ++f) {
    out << "f";
    for (int j = 0; j < 4; ++j) {
      if (j != f->second) {
        out << " " << vertex_map[f->first->vertex(j)];
      }
    }
    out << std::endl;
  }
}

int main()
{
  Triangulation tr;   // 3D-Delaunay triangulation
  Mesh mesh(tr);      // 2D-complex in 3D-Delaunay triangulation
                      // defining the surface
  
  Surface surface(sphere_function(1.0),          // pointer to function
                  Sphere(CGAL::ORIGIN, 2.));     // bounding sphere
  // Note that "2." above is the *squared* radius of the bounding sphere!

  // defining meshing criteria
  CGAL::Surface_mesh_default_criteria_3<Triangulation>
    criteria(30.,     // angular bound
             0.1,     // radius bound
             0.1);    // distance bound

  // meshing surface
  CGAL::make_surface_mesh(mesh, surface, criteria, CGAL::Non_manifold_tag());
  write_to_obj(std::cout, mesh);

  return EXIT_SUCCESS;
}
