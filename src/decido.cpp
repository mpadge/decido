#include <Rcpp.h>
using namespace Rcpp;
#include "earcut_hpp.cpp"

//' earcut
//'
//' earcut description
//'
//' earcut details
//' @param x x-coordinate
//' @param y y-coordinate
//' @param holes index of starting position of each hole in x,y
//' @param numholes the number of holes
//' @export
//' @examples
//' x <- c(0, 0, 0.75, 1, 0.5, 0.8, 0.69)
//' y <- c(0, 1, 1, 0.8, 0.7, 0.6, 0)
//' earcut(x, y)
//' ## hole starts at 9
//' x <- c(0, 0, 0.75, 1, 0.5, 0.8, 0.69, 0, 0.2,
//'       0.5, 0.5, 0.3, 0.2, 0.2)
//' y <- c(0, 1, 1, 0.8, 0.7, 0.6, 0, 0, 0.2,
//'       0.2, 0.4, 0.6, 0.4, 0.2)
//' ind <- earcut(x, y, holes = 8, numholes = 1)
//' plot(x, y, asp = 1)
//' xy <- cbind(x, y)
//' apply(matrix(ind, 3), 2, function(i) polygon(xy[i + 1, ]))
// [[Rcpp::export]]
IntegerVector earcut(NumericVector x, NumericVector y,
                     IntegerVector holes = -1,
                     IntegerVector numholes = 0) {
  using Coord = double;
  // The index type. Defaults to uint32_t, but you can also pass uint16_t if you know that your
  // data won't have more than 65536 vertices.
  using N = uint32_t;
  // Create array
  using Point = std::array<Coord, 2>;
  using Polygon = std::vector<Point>;
  Polygon poly;
  using Polygons = std::vector<Polygon>;
  int vcount = x.length();

  Polygons polyrings;
  for (int ipoint = 0; ipoint < vcount; ipoint++) {
    Point pt = {x[ipoint], y[ipoint]};
    poly.push_back({pt});
    if (numholes[0] > 0) {
      if (ipoint == holes[ipoint]) {
        polyrings.push_back(poly);
        poly.clear();
      }
    }
  }

  polyrings.push_back(poly);
  // Run tessellation
  // Returns array of indices that refer to the vertices of the input polygon.
  // Three subsequent indices form a triangle.
  std::vector<N> indices = mapbox::earcut<N>(polyrings);

  IntegerVector out(indices.size());
  for (int j = 0; j < out.length(); j++){
    out[j] = indices[j];
  }
//  int pcnt =  polyrings.size();
//  int psize = poly.size();
//  out[0] = pcnt;
//  out[1] = psize;
  return out;
}
