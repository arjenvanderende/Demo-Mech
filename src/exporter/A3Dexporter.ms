utility exportsgf "A3D Exporter"
(
  button btnExport "Export selection"
  on btnExport pressed do
  (
    szFilename = GetSaveFileName types:"A3D Models (*.a3d)|*.a3d|All files (*.*)|*.*|"
    if szFilename != undefined then
    (
      out_file = createfile szFilename
      for index = 1 to selection.count do
      (
      -- move object to [0, 0, 0]
    old_pos = selection[index].pos
    selection[index].pos = [0, 0, 0]
    
        tmesh = snapshotAsMesh selection[index]
        format "TRIANGLES %\n" (tmesh.numfaces * 3) to:out_file
        for f = 1 to tmesh.numfaces do
        (
          face = getFace tmesh f
          face_matID = getFaceMatID tmesh f
          normal = meshop.getFaceRNormals tmesh f
		  uvw = getTVFace tmesh f
		  u = getTVert tmesh uvw[1]
		  v = getTVert tmesh uvw[2]
		  w = getTVert tmesh uvw[3]
          
          -- write material id --
          format "%\n" (face_matID) to:out_file

          -- process this face first vertex
          vert = getVert tmesh face.x
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file

          vert_normal = normalize(normal[1]);
          format "% % % " (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file
		  format "% %\n" u[1] u[2] to:out_file
          
          -- process this face second vertex
          vert = getVert tmesh face.y
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file
          vert_normal = normalize(normal[2]);
          format "% % % " (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file
		  format "% %\n" v[1] v[2] to:out_file
		  
          -- process this face third vertex
          vert = getVert tmesh face.z
          format "% % % " (vert.x) (vert.z) (-vert.y) to:out_file
          vert_normal = normalize(normal[3]);
          format "% % % " (vert_normal.x) (vert_normal.z) (-vert_normal.y) to:out_file
 		  format "% %\n" w[1] w[2] to:out_file
		  
--		  format "% % % % % % % %" x[1] x[3] -x[2] k[1] k[3] -k[2] u[2] u[1] to:out_file
--		  format "% % % % % % % %" y[1] y[3] -y[2] l[1] l[3] -l[2] v[2] v[1] to:out_file
--		  format "% % % % % % % %" z[1] z[3] -z[2] m[1] m[3] -m[2] w[2] w[1] to:out_file
        )
    -- move object to old location
    selection[index].pos = old_pos
      )
      close out_file
      messagebox "Export complete!"
    )
  )
)