#include <iostream>
#include <cstdlib>

#include <CGAL/Surface_mesh_default_triangulation_3.h>
#include <CGAL/Complex_2_in_triangulation_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
// default triangulation for Surface_mesher

typedef CGAL::Surface_mesh_default_triangulation_3 Tr;
// c2t3
typedef CGAL::Complex_2_in_triangulation_3<Tr> C2t3;
typedef Tr::Geom_traits GT;
typedef GT::Sphere_3 Sphere_3;
typedef GT::Point_3 Point_3;
typedef GT::FT FT;
typedef FT (*Function)(Point_3);
typedef CGAL::Implicit_surface_3<GT, Function> Surface_3;

FT sphere_function (Point_3 p)
{
  const FT x2=p.x()*p.x(), y2=p.y()*p.y(), z2=p.z()*p.z();
  return x2 + y2 + z2 - 1;
}

void write_to_obj(std::ostream &out, C2t3 &mesh)
{
  std::map<C2t3::Vertex_handle, unsigned> vertex_map;
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
    for (unsigned j = 0; j < 4; ++j) {
      if (j != f->second) {
        out << " " << vertex_map[f->first->vertex(j)];
      }
    }
    out << std::endl;
  }
}

int main() {
  Tr tr;            // 3D-Delaunay triangulation
  C2t3 c2t3 (tr);   // 2D-complex in 3D-Delaunay triangulation
  // defining the surface
  Surface_3 surface(sphere_function,             // pointer to function
                    Sphere_3(CGAL::ORIGIN, 2.)); // bounding sphere
  // Note that "2." above is the *squared* radius of the bounding sphere!
  // defining meshing criteria
  CGAL::Surface_mesh_default_criteria_3<Tr> criteria(30.,  // angular bound
                                                     0.1,  // radius bound
                                                     0.1); // distance bound
  // meshing surface
  CGAL::make_surface_mesh(c2t3, surface, criteria, CGAL::Non_manifold_tag());
  write_to_obj(std::cout, c2t3);

  return EXIT_SUCCESS;
}
