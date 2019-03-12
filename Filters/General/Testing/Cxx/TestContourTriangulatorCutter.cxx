/*=========================================================================

  Program:   Visualization Toolkit
  Module:    TestContourTriangulatorCutter.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// This example demonstrates how to use vtkContourTriangulator
//
// The command line arguments are:
// -I        => run in interactive mode; unless this is used, the program will
//              not allow interaction and exit
// -D <path> => path to the data; the data should be in <path>/Data/

#include "vtkTesting.h"
#include "vtkActor.h"
#include "vtkCellData.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkContourTriangulator.h"
#include "vtkDataSetMapper.h"
#include "vtkCamera.h"
#include "vtkProperty.h"
#include "vtkPlane.h"
#include "vtkCutter.h"
#include "vtkOutlineSource.h"
#include "vtkSmartPointer.h"

#include "vtkCleanPolyData.h"
#include "vtkCompositeDataGeometryFilter.h"
#include "vtkOutputWindow.h"
#include "vtkPlaneCutter.h"
#include "vtkPolyDataReader.h"

int TestContourTriangulatorCutter(int argc, char* argv[])
{
  vtkOutputWindow::SetInstance(vtkSmartPointer<vtkOutputWindow>::New());
  vtkSmartPointer<vtkTesting> testHelper =
    vtkSmartPointer<vtkTesting>::New();
  testHelper->AddArguments(argc, argv);

  std::string tempDir = testHelper->GetTempDirectory();
  std::string tempBaseline=tempDir+"/TestContourTriangulatorCutter.png";

  double bounds[6] = {-210.0, +210.0, -210.0, +210.0, -100.0, +150.0};
  vtkSmartPointer<vtkOutlineSource> outline =
    vtkSmartPointer<vtkOutlineSource>::New();
  outline->SetBounds(bounds);
  outline->GenerateFacesOn();

  vtkNew<vtkPolyDataReader> meshReader;
  meshReader->SetFileName("c:\\Users\\msliv\\Documents\\Segment_1_dec.vtk");

  vtkSmartPointer<vtkPlane> plane =
    vtkSmartPointer<vtkPlane>::New();
  plane->SetNormal(0.0, -1.0, 0.0);
  plane->SetOrigin(0.0, 0.0, 0.0);
  
  vtkNew<vtkPlaneCutter> cutter;
  cutter->BuildTreeOff(); // the cutter crashes for complex geometries if build tree is enabled
  cutter->SetInputConnection(meshReader->GetOutputPort());
  cutter->SetPlane(plane);

  vtkNew<vtkCompositeDataGeometryFilter> geometryFilter; // merge multi-piece output of vtkPlaneCutter
  geometryFilter->SetInputConnection(cutter->GetOutputPort());

  vtkNew<vtkCleanPolyData> pointMerger;
  pointMerger->SetInputConnection(geometryFilter->GetOutputPort());

  vtkSmartPointer<vtkDataSetMapper> cutMapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
  cutMapper->SetInputConnection(cutter->GetOutputPort());
  cutMapper->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> cutActor =
    vtkSmartPointer<vtkActor>::New();
  cutActor->SetMapper(cutMapper);
  cutActor->GetProperty()->SetColor(0,0,0);

  vtkSmartPointer<vtkContourTriangulator> poly =
    vtkSmartPointer<vtkContourTriangulator>::New();
  poly->TriangulationErrorDisplayOn();
  poly->SetInputConnection(pointMerger->GetOutputPort());

  vtkSmartPointer<vtkDataSetMapper> polyMapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
  polyMapper->SetInputConnection(poly->GetOutputPort());
  polyMapper->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> polyActor =
    vtkSmartPointer<vtkActor>::New();
  polyActor->SetMapper(polyMapper);
  polyActor->GetProperty()->SetColor(1.0, 1.0, 1.0);

  // Standard rendering classes
  vtkSmartPointer<vtkRenderer> renderer =
    vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->SetMultiSamples(0);
  renWin->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);

  renderer->AddActor(polyActor);
  renderer->AddActor(cutActor);

  // Standard testing code.
  renderer->SetBackground(0.5,0.5,0.5);
  renWin->SetSize(300,300);

  renWin->Render();
  vtkCamera *camera = renderer->GetActiveCamera();
  renderer->ResetCamera();
  camera->Elevation(-90);

  double origin[3];
  plane->GetOrigin(origin);
  double normal[3];
  plane->GetNormal(normal);
  double newOrigin[3];
  for (double cutPosition = 0.0; cutPosition < 10.0; cutPosition += 0.01)
  {
    newOrigin[0] = origin[0] + cutPosition * normal[0];
    newOrigin[1] = origin[1] + cutPosition * normal[1];
    newOrigin[2] = origin[2] + cutPosition * normal[2];
    plane->SetOrigin(newOrigin);
    std::cout << "Cut plane position: " << newOrigin[0] << ", " << newOrigin[1] << ", " << newOrigin[2] << std::endl;
    renWin->Render();
  }

  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
