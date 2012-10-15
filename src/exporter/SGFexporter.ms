utility exportsgf "SGF Exporter"
(
  button btnExport "Export selection"
  on btnExport pressed do
  (
    szFilename = GetSaveFileName types:"SGF Models (*.sgf)|*.sgf|All files (*.*)|*.*|"
    if szFilename != undefined then
    (
      out_file = createfile szFilename
      for index = 1 to selection.count do
      (
        tmesh = snapshotAsMesh selection[index]
        format "TRIANGLES %\n" (tmesh.numfaces * 3) to:out_file
        format "1.0 1.0 1.0 1.0\n" to:out_file
        for f = 1 to tmesh.numfaces do
        (
          face = getFace tmesh f
          normal = meshop.getFaceRNormals tmesh f
          
          -- process this face first vertex
          vert = getVert tmesh face.x
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file
          vert_normal = normalize(normal[1]);
          format "% % %\n" (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file
          
          -- process this face second vertex
          vert = getVert tmesh face.y
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file
          vert_normal = normalize(normal[2]);
          format "% % %\n" (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file

          -- process this face third vertex
          vert = getVert tmesh face.z
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file
          vert_normal = normalize(normal[3]);
          format "% % %\n" (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file
        )
      )
      close out_file
      messagebox "Export complete!"
    )
  )
)